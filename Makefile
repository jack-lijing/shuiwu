all:server test 
.PHONY:all
.PHONY:server
.PHONY:test
server:
	cd src; make;cd ..
test:
	cd test; make
clean:
	rm [7,8,9,l]* server;cd test;make clean

