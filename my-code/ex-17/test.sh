#! /bin/bash

# Make the script print out what command is executed
# above the output of the command.
set -x

# Make the script abort if one of the commands failed.
set -e

echo "Testing ex17 code..."

# Check if the number of arguments passed to the script
# isn't zero ($# is the number of arguments, same as argc
# in c code).
if [ $# -eq 0 ] ; then
	DB_NAME="test-db"
else
	DB_NAME=$1
fi

# Run some basic commands on the db.
./ex17 $DB_NAME c
./ex17 $DB_NAME s 1 nir geller.nir@gmail.com
./ex17 $DB_NAME s 5 oshrit shoshrit@gmail.com
./ex17 $DB_NAME l
./ex17 $DB_NAME s 3 yair yaya@gmail.com
./ex17 $DB_NAME l
./ex17 $DB_NAME g 3
./ex17 $DB_NAME d 5
./ex17 $DB_NAME l
