/*
 * Listener.cpp
 *
 * TODO This file contains...
 */

#include "listener.h"

Listener::Listener() :
	_stop(false)
{
}

Listener::~Listener()
{
	this->stop();
}

int Listener::start(unsigned int port)
{
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0); // Internet stream socket with auto-detected protocol
	if (_socket_fd < 0)
	{
		fprintf(stderr, "ERROR: Unable to open socket\n");
		return 1; // Error
	}

	// Set up listening address and port
	memset((char *) &_server_address, 0, sizeof(_server_address));
	_server_address.sin_family = AF_INET;
	_server_address.sin_port = htons(port);
	_server_address.sin_addr.s_addr = INADDR_ANY;

	// Start listening. NOTE: This may fail if another listener was recently closed
	// on this socket, as TCP sockets may be kept alive for up to 4 minutes.
	if (bind(_socket_fd, (struct sockaddr *)&_server_address, sizeof(_server_address)) < 0)
	{
		fprintf(stderr, "ERROR: Unable to bind port\n");
		return 1; // Error
	}

	// Open the socket
	listen(_socket_fd, 5);
	return 0; //Success
}

Connection *Listener::accept_client()
{
	struct sockaddr_in client_address;
	unsigned int client_length = sizeof(client_address);

	// Attempt to accept an incoming connection. This is blocking.
	int connection_fd = accept(_socket_fd, (struct sockaddr *)&client_address, &client_length);
	if (connection_fd < 0)
	{
		return NULL; // Error
	}

	return new Connection(connection_fd, client_length, client_address);
}

void Listener::stop()
{
	close(_socket_fd);
}
