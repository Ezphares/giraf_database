CC=g++
CCFLAGS=-c -Wall
LMYSQL=$$(mysql_config --libs)
CMYSQL=$$(mysql_config --include)
OUTPUT=serverapp

all : server

server : program.o
	$(CC) program.o -o $(OUTPUT)

program.o : program.cpp
	$(CC) $(CCFLAGS) program.cpp

clean :
	rm -f *.o $(OUTPUT)

