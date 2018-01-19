#! /bin/zsh
if [ $# -eq 0]
then
	echo 'No Variables passed to script!'
	exit -1
else
	mkdir ex-$1
	echo 'CFLAGS=-Wall -g\n' > ex-$1/Makefile
fi
