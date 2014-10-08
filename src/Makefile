all:
	gcc -g -o ../server main_linux.c  protocol.c do_mysql.c csapp.c -I ../include/ -lpthread `mysql_config --cflags --libs`
	gcc -g  protocol.c do_stub.c csapp.c -shared -fPIC -o ../libprot.so
	gcc -g  -o ../main_nc_so1 main_nc_so1.c -ldl
so:
	gcc -g  protocol.c do_stub.c csapp.c -shared -fPIC -o ../libprot.so
nc1:
	gcc -g  -o ../main_nc_so1 main_nc_so1.c -ldl
