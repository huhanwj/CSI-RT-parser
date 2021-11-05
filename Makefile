SENDOBJS = csi_fun.o send.o
RECVOBJS = csi_fun.o recv.o
CC = gcc
recv: $(RECVOBJS)
	$(CC) $(RECVOBJS) -o recv
send: $(SENDOBJS)
	$(CC) $(SENDOBJS) -o send
csi_fun.o: csi_fun.c csi_fun.h
	$(CC) -c csi_fun.c -o csi_fun.o
send.o: send.c csi_fun.h
	$(CC) -c send.c -o send.o
recv.o: recv.c csi_fun.h
	$(CC) -c recv.c -o recv.o
clean: 
	rm -f *.o recv send

