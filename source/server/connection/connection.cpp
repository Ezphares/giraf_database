/*
 * connection.cpp
 *
 * TODO This file contains...
 */

#include "connection.h"

Connection::Connection() :
		_connected(false)
{

}

Connection::Connection(int fd, unsigned int length, sockaddr_in address) :
		_connection_fd(fd),
		_connection_length(length),
		_connection_address(address),
		_connected(true)
{

}

Connection::~Connection() {
	// TODO Auto-generated destructor stub
}

