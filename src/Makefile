all:server client test89
server:
	gcc -g -o echoservert main_linux.c  protocol.c do-stub.c log.c csapp.c -I ../include/ -lpthread `mysql_config --cflags --libs`
client:
	gcc -g -o testcli testcli.c log.c
test89:
	gcc -g -o test89 test89.c log.c
clean:
	rm echoservert testcli test89
