#! /bin/zsh
# A basic script for creating the next exercise directory.
# Might change in order to adapt to higher level exercises.
number_re='^[0-9]+'
makefile="CFLAGS=-Wall -g\n\nall: ex"$1"\n\nclean:\n\trm -rf ex"$1
if [[ $# -eq 0 ]] ; then
	echo 'No Variables passed to script!'
	exit -1
else
	if [[ $1 =~ $number_re ]] ; then
		echo 'Creating directory ex-'$1
		mkdir ex-$1
		echo $makefile > ex-$1/Makefile
	else
		echo 'First argument must be a number!'
	fi
fi
