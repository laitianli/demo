RPC_SRV=rpc_server
RPC_CLT=rpc_client

INC = -I/usr/include/tirpc
LIBS = -ltirpc

all: rpcgen $(RPC_SRV) $(RPC_CLT)


rpcgen:
	rpcgen -C date.x

$(RPC_SRV):
	gcc -o $@ dateproc.c date_svc.c $(INC) $(LIBS) -g

$(RPC_CLT):
	gcc -o $@ date_clnt.c rdate.c $(INC) $(LIBS) -g

clean:
	rm -f $(RPC_SRV) $(RPC_CLT) *.o date_svc.c date_clnt.c date.h
