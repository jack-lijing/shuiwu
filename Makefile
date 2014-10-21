all:server test 
.PHONY:all
.PHONY:server
.PHONY:test
server:
	cd src; make;cd ..
test:
	cd test; make;cd ..
clean:
	find ./ | egrep -v "src|Makefile|test|\.git" | xargs rm;cd test;make clean

