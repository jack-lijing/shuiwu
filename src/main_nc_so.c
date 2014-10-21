#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <fcntl.h>
//#include "water.h"

int main(void)
{
	void * handler = dlopen("./libprot.so", RTLD_NOW);
	assert(handler != NULL);
	void (*protocol)(int, void *) = dlsym(handler, "protocol");
	assert(protocol != NULL);
	
	int fd, ret;
	char buffer[1024];
	unlink("/tmp/FIFO");
	assert(mkfifo("/tmp/FIFO", 0666) == 0);

	fd = open("/tmp/FIFO",O_RDWR) ;

	while (1)
	{
		(*protocol)(fd, NULL);
		read(fd, buffer,1024);
		printf("%s", buffer);

	}
	dlclose(handler);
	return 0;

}
