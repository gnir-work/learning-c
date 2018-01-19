#! /bin/zsh
# A basic script for creating the next exercise directory.
# Might change in order to adapt to higher level exercises.
number_re='^[0-9]+'
if [[ $# -eq 0 ]] ; then
	echo 'No Variables passed to script!'
	exit -1
else
	if [[ $1 =~ $number_re ]] ; then
		echo 'Creating directory ex-'$1
		mkdir ex-$1
		echo 'CFLAGS=-Wall -g\n' > ex-$1/Makefile
	else
		echo 'First argument must be a number!'
	fi
fi
