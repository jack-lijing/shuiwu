#!/bin/bash
[ $# -ne  1 ] && echo "Usage: $0 file.log"  && exit 0
./servert 88 &
for i in 1 2 3 4 5 6 
do
	cat $1 | egrep 'Recv=.{5}700'${i} | awk 'BEGIN {FS="["} {print $2}' | tr -d "]"  | tail -n 1 | nc jack60 88
done
sudo netstat -lnop | grep servert | awk '{print $7}' | tr -d '[:alpha:]./_' | xargs sudo kill
