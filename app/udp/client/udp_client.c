#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUF_SIZE 3000
void error_handling(char* message);
#define GTPU_HEAD_LEN 16

void gtpu_packet_build_16bytes(char* data, unsigned int data_len, unsigned int teid)
{
	unsigned int len = data_len + 8;
	data[0] = 0x34;
	data[1] = 0xff;
	data[2] = len >> 8;
	data[3] = len & 0xff;
	data[4] = (teid >> 24) & 0xff;
	data[5] = (teid >> 16) & 0xff;
	data[6] = (teid >> 8) & 0xff;
	data[7] = (teid     ) & 0xff;
	data[8] = 0x00;
	data[9] = 0x00;
	data[10] = 0x00;
	data[11] = 0x85;
	data[12] = 0x01;
	data[13] = 0x00;
	data[14] = 0x01;
	data[15] = 0x00;
}

unsigned int get_gtpu_id(unsigned char* data)
{
	unsigned int id = 0;
	id = (data[4] << 24)|(data[5] << 16) | (data[6] << 8) | data[7];
	return id;
}

int main(int argc, char* argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t adr_sz;
	unsigned int gtpu_id = 0;
    struct sockaddr_in serv_adr, from_adr;
    if (argc < 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
	if (argc == 5 && strcmp(argv[3], "-gtpu") == 0) {
		gtpu_id = atoi(argv[4]);	
		printf("[Note] gtpu teid: %d\n", gtpu_id);
	}
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        error_handling("socket() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));
	int input_len = 0;
	char tmp_buf[BUF_SIZE - GTPU_HEAD_LEN - 1] = {0};
    while (1)
    {
        fputs("Insert message(q to quit): ", stdout);
		if (gtpu_id > 0) {			
			fgets(tmp_buf, sizeof(tmp_buf), stdin);
			input_len = strlen(tmp_buf);
			
			if (!strcmp(tmp_buf, "q\n") || !strcmp(tmp_buf, "Q\n"))
			{
				break;
			}
			if (!strcmp(tmp_buf, "big\n"))
			{
				int i = 0;
				for (i = 0; i < 1600; i ++) {
					tmp_buf[i] = 'a' + (i + 1) % 26;
				}
				input_len = 1600;
			}
			gtpu_packet_build_16bytes(message, input_len, gtpu_id);
			memcpy(message + GTPU_HEAD_LEN, tmp_buf, input_len);
			input_len += GTPU_HEAD_LEN;
		}
		else {
        	fgets(message, sizeof(message), stdin);
			
	        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
	        {
	            break;
	        }
			input_len = strlen(message);
		}

        int sendlen = sendto(sock, message, /*strlen(message)*/input_len, 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
		printf("sendlen:%d\n", sendlen);
		adr_sz = sizeof(from_adr);
		memset(message, 0, sizeof(message));
        str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&from_adr, &adr_sz);
		if (str_len > 0) {
	        message[str_len] = 0;
			if (gtpu_id > 0)
				printf("[GTPU: %u] Message from server: %s\n", get_gtpu_id((unsigned char*)message), message + GTPU_HEAD_LEN);
			else
	        	printf("Message from server: %s\n", message);
		}
    }
    close(sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
