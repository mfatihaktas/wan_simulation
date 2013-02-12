#!/bin/bash
echo $1
if [ $1 = 'p' ]
then
	make producer
	./producer 127.0.0.1 1210 #for now
elif [ $1 = 'g' ]
then
	make gateway
	./gateway
elif [ $1 = 's' ]
then
	make server
	./server
elif [ $1  = 'c' ]
then
	make client
	./client 127.0.0.1 #for now
elif [ $1  = 'd' ]
then
	make dsanode
	./dsanode #for now
else
	echo "Argument did not match !"
fi
