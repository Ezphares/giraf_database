# INFO
# Make sure mysql_config from `MySQL Connector/C` is on PATH

# SETTINGS
# set to local BOOST directory
DIR_BOOST=/usr/local/boost_1_53_0
# name of binary
OUTPUT=serverapp
# END SETTINGS

# MYSQL
LMYSQL=`mysql_config --libs`
CMYSQL=`mysql_config --include`

# BOOST
LBOOST=-L$(DIR_BOOST)/libs -L$(DIR_BOOST)/stage/lib
CBOOST=-I$(DIR_BOOST)

CC=g++
CFLAGS=-c -O0 -g3 -Wall $(CMYSQL) $(CBOOST)
LDFLAGS=$(LMYSQL) $(LBOOST) -ljson -lpthread

CONNECTION_OBJECTS=connection.o listener.o framework.o
CONNECTION_SOURCES=connection/connection.cpp \
                   connection/listener.cpp \
                   connection/framework.cpp
DATABASE_OBJECTS=queryresult.o database.o
DATABASE_SOURCES=database/queryresult.cpp \
                 database/database.cpp
API_OBJECTS=api.o api_read.o api_create.o api_delete.o api_update.o builders.o
API_SOURCES=api/api.cpp \
	    api/api_read.cpp \
	    api/api_create.cpp \
	    api/api_delete.cpp \
	    api/api_update.cpp \
	    api/builders.cpp
	    
MODULES=program.o m_database m_connection m_api
TESTS=t_connection t_database

OBJECTS=program.o $(CONNECTION_OBJECTS) $(DATABASE_OBJECTS) $(API_OBJECTS)

all : server clean

server : $(MODULES) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(OUTPUT)

program.o : program.cpp
	$(CC) $(CFLAGS) program.cpp

m_connection : $(CONNECTION_SOURCES)
	$(CC) $(CFLAGS) $(CONNECTION_SOURCES)

m_database : $(DATABASE_SOURCES)
	$(CC) $(CFLAGS) $(DATABASE_SOURCES)

m_api : $(API_SOURCES)
	$(CC) $(CFLAGS) $(API_SOURCES)

clean :
	rm -f *.o

test: $(TESTS)

t_connection :
	$(CC) $(CONNECTION_SOURCES) connection/test.cpp $(LDFLAGS) -o test_connection

t_database :
	$(CC) $(DATABASE_SOURCES) database/test.cpp $(LDFLAGS) -o test_database

