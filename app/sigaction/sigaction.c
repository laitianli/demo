#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>

static void sig_int_handle(int sig, siginfo_t* info, void* ut)
{
	switch(sig)
	{
		case SIGINT:
			printf("recevie SIGINT signal.\n");
			break;
		case SIGSEGV:
			printf("recevie SIGSEGV signal.\n");
			break;
		default:
			break;
	}
}


int main(int argv, char** argc)
{

	struct sigaction sa = {0};
	sigemptyset(&sa.sa_mask);
//	sigfillset(&sa.sa_mask);
//	sigdelset(&sa.sa_mask,SIGINT);
//	sigdelset(&sa.sa_mask,SIGSEGV);
	sa.sa_flags = SA_SIGINFO | SA_NODEFER;
	sa.sa_sigaction = sig_int_handle;

	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGSEGV, &sa, NULL);
	//pthread_t threadID = 0;

	//int ret = pthread_create(&threadID, NULL, );

	int i = 0;
	while (i++ < 20)
		sleep(1);

	return 0;
}

