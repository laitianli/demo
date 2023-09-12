#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <net/if.h>
#include <linux/if_tun.h>
#include <sys/epoll.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#define LOG_ERR(format, ...) printf ("[Error][%s:%d]"format"\n", __func__, __LINE__, ##__VA_ARGS__)
#define LOG_DBG(format, ...) printf ("[Debug][%s:%d]"format"\n", __func__, __LINE__, ##__VA_ARGS__)
static int tun_fd = -1;
static int rx_task_is_running = 1;
static int exit_threads = 0;
static void usr1_handler(int signo)
{
    exit_threads = 1;
	rx_task_is_running = 0;
}


static int tap_alloc(char *dev, int flags) {

	struct ifreq ifr;
	int fd, err;
	const char *clonedev = "/dev/net/tun";

	if( (fd = open(clonedev, O_RDWR)) < 0 ) {
		LOG_ERR("open failed");
		return fd;
	}

	memset(&ifr, 0, sizeof(ifr));

	ifr.ifr_flags = flags; 
	if (*dev) {
		strncpy(ifr.ifr_name, dev, IFNAMSIZ);
	}

	if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ) {
		LOG_ERR("ioctl failed");
		close(fd);
		return -1;
	}

	strcpy(dev, ifr.ifr_name);

	ioctl(fd, TUNSETNOCSUM, 1);

	ioctl(fd, TUNSETDEBUG, 1);
	return fd;
}


int setup_tuntap_device(int port)
{
	int fd;
	struct ifreq ifr;
	int gen_fd;
	char fp_name[IFNAMSIZ];
	struct sockaddr hwaddr;

	memset(&hwaddr, 0x0, sizeof(hwaddr));

	snprintf(fp_name, IFNAMSIZ, "ttap%d", port);
	fp_name[IFNAMSIZ - 1] = 0;

	fd = tap_alloc(fp_name, IFF_TAP  | IFF_NO_PI);
	if (fd < 0) {
		LOG_ERR("tap_alloc failed");
		return -1;
	}
	unsigned char mac[6] = {0x00, 0xab, 0x11, 0x12, 0x13, 0x14};
	hwaddr.sa_family = AF_UNIX;
	memcpy(hwaddr.sa_data, mac, sizeof(mac));

	memset(&ifr, 0x0, sizeof(ifr));
	strncpy(ifr.ifr_name, fp_name, IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ - 1] = 0;
	memcpy(&ifr.ifr_hwaddr, &hwaddr, sizeof(ifr.ifr_hwaddr));


	if (ioctl(fd, SIOCSIFHWADDR, &ifr) < 0) {
		LOG_ERR("Failed to set MAC address: %s", strerror(errno));
		close(fd);
		return -1;
	}

	gen_fd = socket(PF_INET, SOCK_DGRAM, 0);

	memset(&ifr, 0x0, sizeof(ifr));
	strncpy(ifr.ifr_name, fp_name, IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ - 1] = 0;
	ifr.ifr_mtu = 1500;
	LOG_DBG("Fastpath device %s MTU %i", fp_name, ifr.ifr_mtu);

	if (ioctl(gen_fd, SIOCSIFMTU, &ifr) < 0) {
		LOG_ERR("Failed to set MTU: %s", strerror(errno));
		close(gen_fd);
		close(fd);
		return -1;
	}

	memset(&ifr, 0x0, sizeof(ifr));
	strncpy(ifr.ifr_name, fp_name, IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ - 1] = 0;
	if (ioctl(gen_fd, SIOCGIFFLAGS, &ifr) < 0) {
		LOG_ERR("Failed to get interface flags: %s", strerror(errno));
		close(gen_fd);
		close(fd);
		return -1;
	}

	if (!(ifr.ifr_flags & IFF_UP)) {
		ifr.ifr_flags |= IFF_UP | IFF_NOARP;
		if (ioctl(gen_fd, SIOCSIFFLAGS, &ifr) < 0) {
			LOG_ERR("Failed to set interface flags: %s",
					strerror(errno));
			close(gen_fd);
			close(fd);
			return -1;
		}
	}

	memset(&ifr, 0x0, sizeof(ifr));
	strncpy(ifr.ifr_name, fp_name, IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ - 1] = 0;
	if (ioctl(gen_fd, SIOCGIFINDEX, &ifr) < 0) {
		LOG_ERR("Failed to get interface index: %s", strerror(errno));
		close(gen_fd);
		close(fd);
		return -1;
	}
	close(gen_fd);
	LOG_DBG("fd: %d", fd);
	return fd;
}

static void* tun_rx_thread(void* arg)
{
	fd_set read_fd;
	struct timeval timeout;
	int len = 0;
	int r = 0;
	char buf[1518] = {0};

	do {
		memset(buf, 0, sizeof(buf));

retry:
		FD_ZERO(&read_fd);
		FD_SET(tun_fd, &read_fd);

		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		r = select(tun_fd + 1, &read_fd, NULL, NULL, &timeout);
		if (!rx_task_is_running)
			break;
		if (r == 0) {
			printf("select timeout \n");
			goto retry;
		}

		if (r < 0) {
			LOG_ERR("select failed, (errno:%d, len:%d)", errno, len);
			break;
		}

		len = read(tun_fd, buf, sizeof(buf));
		if (len <= 0) {
			LOG_ERR("read failed, [errno: %d, len: %d] tun_fd:%d, r: %d", errno, len, tun_fd, r);
			if (!rx_task_is_running) {
				break;
			}
			sleep(1);
			goto retry;
		}
		LOG_DBG("read len:%d", len);
	} while(1 == rx_task_is_running);
	return NULL;
}

int main(int argv, char** argc)
{
	signal(SIGUSR1, usr1_handler);
	tun_fd = setup_tuntap_device(0);
	if (tun_fd <= 0)
		return -1;
	pthread_t rx_tid;
	int ret = pthread_create(&rx_tid, NULL, tun_rx_thread, NULL);
	if (ret < 0) {
		LOG_ERR("pthread_create failed");
		return -1;
	}
	while(exit_threads == 0)
		sleep(1);
	
	LOG_DBG("begin call close(fd) %d", tun_fd);
	close(tun_fd);
	LOG_DBG("after call close(fd) %d", tun_fd);
	return 0;
}
