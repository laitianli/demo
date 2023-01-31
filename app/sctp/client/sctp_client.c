#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
 
const int buf_size = 1024;
const int echo_port = 2014;
 
char *usage_msg = "usage echo_c ip_addr istreams ostream ochannel";
void usage()
{
	fprintf(stderr,"%s\n", usage_msg);
	exit(1);
}
 
int main(int argc, char** argv)
{
	int sockfd, len;
	struct sockaddr_in serv_addr;
	struct sockaddr_in *addrs;
	int addr_size = 1;
 
	char* msg = "hello world.\n";
	struct sctp_initmsg initmsg;
	struct sctp_status status;
	struct sctp_sndrcvinfo sinfo;
	int ochannel;
	sctp_assoc_t assoc_id;
	int brst;
 
	//if(argc < 5) usage();
	sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_SCTP);
	if(sockfd < 0)
	{
		fprintf(stderr,"error while initializing socket.\n");
		exit(1);
	}
 #if 0
	addrs = malloc(sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("192.168.42.129");
	serv_addr.sin_port = htons(echo_port);
    memcpy(addrs,&serv_addr,sizeof(struct sockaddr_in));
#else
    addrs = malloc(sizeof(struct sockaddr_in) * 2);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("192.168.42.129");
	serv_addr.sin_port = htons(echo_port);
    memcpy(&addrs[1],&serv_addr,sizeof(struct sockaddr_in));

    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("192.168.3.200");
	serv_addr.sin_port = htons(echo_port);
    memcpy(&addrs[0],&serv_addr,sizeof(struct sockaddr_in));
#endif
	
	memset(&initmsg, 0, sizeof(initmsg));
	initmsg.sinit_max_instreams = 16;
	initmsg.sinit_num_ostreams = 16;
	fprintf(stdout,"asking for %d input stream, %d output stream.\n",
		 initmsg.sinit_max_instreams,initmsg.sinit_num_ostreams);
 
	brst = setsockopt(sockfd, IPPROTO_SCTP,SCTP_INITMSG,
		 &initmsg,sizeof(initmsg));
	if(brst != 0)
	{
		fprintf(stderr,"error while set socket option.\n");
		exit(1);
	}
#if 0
	brst = sctp_connectx(sockfd, (struct sockaddr*) addrs, 1, &assoc_id);
#else
    brst = sctp_connectx(sockfd, (struct sockaddr*)addrs, 2, &assoc_id);
#endif
	if(brst < 0)
	{
		fprintf(stderr, "can not connect to host.\n");
		exit(1);
	}
	fprintf(stderr, "assoc_id %d.\n", assoc_id);
	memset(&status,0,sizeof(status));
	len = sizeof(status);
	status.sstat_assoc_id = 1;
 
	brst = getsockopt(sockfd, IPPROTO_SCTP,SCTP_STATUS,&status,&len);
	if(brst < 0)
	{
		fprintf(stderr,"error while geting socket option.\n");
		exit(1);
	}
 
	fprintf(stdout,"input stream:%d, output stream:%d\n",
		 status.sstat_instrms,status.sstat_outstrms);
 
	ochannel = 4;
	if(ochannel >= status.sstat_outstrms)
	{
		fprintf(stdout,"writing on illegal channel %d \n", ochannel);
	}
 
	bzero(&sinfo,sizeof(sinfo));
	sinfo.sinfo_stream = ochannel;
    int count = 10;
    if (argc > 1) {
        count = atoi(argv[1]);
    }
    printf("send packet count: %d\n", count);
    while (count-- > 0) {
	    sctp_send(sockfd, msg, strlen(msg), &sinfo, 0);
        sleep(1);
    }
//	sinfo.sinfo_flags = SCTP_EOF;
//    sctp_send(sockfd,NULL,0,&sinfo,0);
    
	close(sockfd);
	return 0;
}


