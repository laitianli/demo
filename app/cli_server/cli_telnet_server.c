#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <poll.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

static int cli_serv_fd = -1, cli_tmp_fd = -1;
static int close_cli;

static char telnet_echo_off[] = {
	0xff, 0xfb, 0x01, /* IAC WILL ECHO */
	0xff, 0xfb, 0x03, /* IAC WILL SUPPRESS_GO_AHEAD */
	0xff, 0xfd, 0x03, /* IAC DO SUPPRESS_GO_AHEAD */
};

static void sendstr(int fd, const char *s)
{
	if (S_ISSOCK(fd))
		send(fd, s, strlen(s), 0);
	else
		(void)(write(fd, s, strlen(s)) + 1);
}

void sendcrlf(int fd)
{
#if 0
	if ((conn->status & DO_ECHO) == 0)
		sendstr(conn, "\n"); /* no extra prompts */
	else if (conn->status & ENABLED_OK)
		sendstr(conn, "\r\n# ");
	else
#endif
	sendstr(fd, "\r\n> ");
}

static void cli_send_welcome_banner(int fd)
{
	char sendbuf[512];
	sprintf(sendbuf,
		"\r\n"
		"--==--==--==--==--==--==--==--==--==--==--\r\n"
		"------ WELCOME to DPDK-Pktgen CLI ------\r\n"
		"--==--==--==--==--==--==--==--==--==--==--\r\n"
		);
	sendstr(fd, sendbuf);
	sendcrlf(fd);
}


static void cli_sa_accept(int fd)
{
#if 0
	struct cli_conn *conn;

	conn = &connection;
	bzero(conn, sizeof(*conn));
	conn->fd = fd;
#endif
	send(fd, telnet_echo_off, sizeof(telnet_echo_off), 0);

	printf("new sock %d opened\r\n", fd);
}

int
cli_read(int fd, char *buf, int len)
{
	int n = 0;

	if (!buf || !len)
		return 0;

	while(len--)
		n += read(fd, buf++, 1);
	return n;
}

/* Poll the I/O routine for characters */
int
cli_my_poll(char *c, int fd)
{
	struct pollfd fds;

	fds.fd      = fd;
	fds.events  = POLLIN;
	fds.revents = 0;

	//cli_use_timers();

	if (poll(&fds, 1, 0)) {
		if ((fds.revents & (POLLERR | POLLNVAL)) == 0) {
			if ((fds.revents & POLLHUP)) {
				printf("POLLHUP event, will exit");
				return 0;
			}
			else if ((fds.revents & POLLIN)) {
				int n = read(fds.fd, c, 1);
				printf("c:%c\n", *c);
				if (n > 0)
					return 1;
			}
		}
	}
	return 0;
}

#include <emmintrin.h>
static inline void rte_pause(void)
{
	_mm_pause();
}

static void cli_server(void* arg)
{
    int alen;
    struct sockaddr_in my_addr, caller;
    int reuse = 1;
    fd_set read_fd, fds;
	int select_nfds;
	
	cli_serv_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (cli_serv_fd < 0) {
		printf("cli serv socket\n");		
		return -1;
	}
	if (setsockopt(cli_serv_fd, SOL_SOCKET,
		   SO_REUSEADDR, (void *)&reuse, sizeof(reuse)) < 0)
		printf("cli setsockopt (SO_REUSEADDR)\n");
	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(9988);
	my_addr.sin_addr.s_addr = htonl(0);

	if (bind(cli_serv_fd, (struct sockaddr *)&my_addr,
		 sizeof(struct sockaddr)) < 0) {
		printf("serv bind\n");
		return -1;
	}

	listen(cli_serv_fd, 1);

	FD_ZERO(&read_fd);
	FD_SET(cli_serv_fd, &read_fd);
	while (1) {
		struct timeval timeout;
		int r;

		fds = read_fd;
		select_nfds = cli_serv_fd + 1;

		if (cli_tmp_fd > 0) {
			FD_SET(cli_tmp_fd, &fds);
			if (cli_tmp_fd > select_nfds - 1)
				select_nfds = cli_tmp_fd + 1;
		}

		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		r = select(select_nfds, &fds, NULL, NULL, &timeout);

		if (close_cli) {
			if (cli_tmp_fd > 0)
				close(cli_tmp_fd);
			cli_tmp_fd = -1;
			close_cli = 0;
			printf("CLI connection closed\r\n");
		}

		if (r < 0)
			continue;

		if (FD_ISSET(cli_serv_fd, &fds)) {
			close_cli = 0;

			if (cli_tmp_fd > 0)
				close(cli_tmp_fd);

			alen = sizeof(caller);
			cli_tmp_fd = accept(cli_serv_fd,
					(struct sockaddr *)&caller,
					(socklen_t *)&alen);
			if (cli_tmp_fd < 0) {
				printf("cli serv accept\n");
				continue;
			}
			cli_sa_accept(cli_tmp_fd);
			cli_send_welcome_banner(cli_tmp_fd);
			printf("CLI connection established\r\n");
		}

		if (cli_tmp_fd > 0 && FD_ISSET(cli_tmp_fd, &fds)) {
			char c;
			while (1) {
				cli_my_poll(&c, cli_tmp_fd);
				//printf("00000000000\n");
				rte_pause();
				///printf("11111111111\n");
			}
		}
	} /* while () */

	if (cli_tmp_fd > 0)
		close(cli_tmp_fd);
	cli_tmp_fd = -1;

	close(cli_serv_fd);
	cli_serv_fd = -1;
}

pthread_t cli_create_thread(void)
{
	pthread_t tid;
	pthread_create(&tid, NULL, cli_server, NULL);
	return tid;
}

int main(int argc, char** argv)
{
  // cli_server();
  pthread_t tid = cli_create_thread();
  pthread_join(tid, NULL);
}
