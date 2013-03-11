/*
 * connection.cpp
 *
 * TODO This file contains...
 */

#include "connection.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <sstream>
#include <poll.h>

#define BUFFER_SIZE 5

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

Connection::~Connection()
{
	this->disconnect();
}

int Connection::connect_to_host(const char *ip, unsigned int port)
{
	struct sockaddr_in address;

	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, (void *)&address.sin_addr);
	address.sin_port = htons(port);

	_connection_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (connect(_connection_fd, (sockaddr *)&address, sizeof(address)) < 0)
	{
		fprintf(stderr, "ERROR: Could not connect to server\n");
		return 1;
	}

	_connected = true;
	return 0;
}

int Connection::send(const char *message)
{
	if (!this->is_connected())
	{
		fprintf(stderr, "ERROR: Attempt to write to socket with no connection");
		return 1;
	}

	int l = strlen(message), n;
	n = write(_connection_fd, message, l);
	if (n < 0)
	{
		fprintf(stderr, "ERROR: Writing to socket");
		return 1;
	}
	return 0;
}

const char *Connection::receive()
{
	if (!this->is_connected())
	{
		fprintf(stderr, "ERROR: Attempt to read from socket with no connection");
		return NULL;
	}

	pollfd p;
	p.fd = _connection_fd;
	p.events = POLLIN;

	poll(&p, 1, -1); // Wait for data

	char *buffer =(char *)malloc(BUFFER_SIZE);

	std::stringstream msg;

	do
	{
		memset((void *)buffer, 0, BUFFER_SIZE);
		read(_connection_fd, buffer, BUFFER_SIZE - 1);
		msg << buffer;
		poll(&p, 1, 100); // See if more data is available
	}
	while (p.revents & POLLIN);

	free(buffer);

	return msg.str().c_str();
}

void Connection::disconnect()
{
	_connected = false;
	close(_connection_fd);
}

bool Connection::is_connected()
{
	return _connected;
}
