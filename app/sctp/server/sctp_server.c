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
 
char* usage_msg="usage: sctp_echo [ip_addr]+ istreams ostreams";
void usage()
{
    fprintf(stderr,"%s\n",usage_msg);
    exit(1);
}
 
int main(int argc, char** argv)
{ 
    char buf[buf_size];
    time_t time;
 
    int sockfd, client_sockfd,flags;
    int nread,len;
    struct sockaddr_in serv_addr, client_addr;
    struct sctp_initmsg initmsg;
    struct sctp_status status;
    sctp_assoc_t associd;
    struct sctp_sndrcvinfo sinfo;
    struct sctp_event_subscribe events;
 
    struct sockaddr_in *addrs;
    int addr_size = sizeof(struct sockaddr_in);
    int addr_count = 1;
    int num = 0;
 
    //if(argc < 4) usage();
 
    sockfd = socket(AF_INET,SOCK_STREAM,IPPROTO_SCTP);
    if(sockfd < 0)
    {
        fprintf(stderr,"socket descriptor is invalid.");
        exit(1);
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(echo_port);
#if 1 
    int brst = sctp_bindx(sockfd,(struct sockaddr*)&serv_addr,
                addr_count,SCTP_BINDX_ADD_ADDR);
#else
    int len_addr = sizeof(struct sockaddr);
    int brst = bind(sockfd, (struct sockaddr*)&serv_addr, len_addr);
#endif
    if(brst == -1)
    {
        fprintf(stderr,"error while binding address.");
        exit(1);
    }
 
    memset(&initmsg,0,sizeof(initmsg));
    initmsg.sinit_max_instreams = 15;
    initmsg.sinit_num_ostreams = 15;
    fprintf(stdout,"asking for input streams:%d, output streams %d\n",
            initmsg.sinit_max_instreams, initmsg.sinit_num_ostreams);
    
    brst = setsockopt(sockfd,IPPROTO_SCTP,SCTP_INITMSG,
            &initmsg,sizeof(initmsg));
    if(brst != 0)
    {
        fprintf(stderr,"error while setting sctp options.\n");
        exit(1);
    }
 
    listen(sockfd,5);
 
    for(;;)
    {
        len = sizeof(client_addr);
        client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &len);
        if(client_sockfd == -1)
        {
            fprintf(stderr,"error while accepting connection, continue.\n");
            continue;
        }
		printf("struct sockaddr_in size[%lu] len[%d]\n", sizeof(struct sockaddr_in), len);
        memset(&status,0,sizeof(status));
        len = sizeof(status);
        status.sstat_assoc_id = 0;
 
        brst = getsockopt(client_sockfd,IPPROTO_SCTP,SCTP_STATUS,&status,&len);
        if(brst == -1)
        {
            fprintf(stderr,"error while getting socket option.\n");
        }
 
        fprintf(stdout,"input stream:%d, output stream:%d\n",
                status.sstat_instrms, status.sstat_outstrms);
        for(;;)
        {
            len = sizeof(&sinfo,sizeof(sinfo));
			printf("sinfo len[%d]\n", len);
            memset(buf,0,buf_size);
            nread = sctp_recvmsg(client_sockfd,buf,buf_size,
                    (struct sockaddr*)&client_addr, &len, &sinfo, &flags);
            if(nread <= 0) break;
            fprintf(stdout,"%s\n",buf);
            fprintf(stdout,"read %d bytes on channel %d\n", 
                    nread, sinfo.sinfo_stream); 
        }
		printf("client close .\n");
        close(client_sockfd);
    }
 
    return 0;
}
