# INFO
# Make sure mysql_config from `MySQL Connector/C` is on PATH

# SETTINGS
# set to local BOOST directory
DIR_BOOST=/usr/local/boost_1_53_0
# name of binary
OUTPUT=serverapp
# END SETTINGS

# MYSQL
LMYSQL=$$(mysql_config --libs)
CMYSQL=$$(mysql_config --include)

# BOOST
LBOOST=-L$(DIR_BOOST)/libs -L$(DIR_BOOST)/stage/lib
CBOOST=-I$(DIR_BOOST)

CC=g++
CFLAGS=-c -Wall $(CMYSQL) $(CBOOST)
LDFLAGS=-lpthread $(LBOOST) $(LMYSQL)

all : server

server : program.o
	$(CC) $(LDFLAGS) program.o -o $(OUTPUT)

program.o : program.cpp
	$(CC) $(CFLAGS) program.cpp

clean :
	rm -f *.o $(OUTPUT)

