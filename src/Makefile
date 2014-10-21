all:server so test nc
server:
	gcc -g -o ../server main_linux.c  protocol.c do_mysql.c csapp.c -I ../include/ -lpthread `mysql_config --cflags --libs`
so:
	gcc -g  protocol.c do_stub.c csapp.c -shared -fPIC -o ../libprot.so
nc:
	gcc -g  -o ../main_nc_so main_nc_so.c -ldl
test:
	gcc -g -o ../test/servert main_linux.c  protocol.c do_stub.c csapp.c -I ../include/ -lpthread `mysql_config --cflags --libs`
