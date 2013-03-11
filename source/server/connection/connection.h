/*
 * connection.h
 *
 * TODO This file contains...
 */

#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <cstdlib>

class Connection {
public:
	Connection();
	Connection(int fd, unsigned int length, struct sockaddr_in address);
	virtual ~Connection();

public:
	int connect_to_host(const char *hostname, unsigned int port);
	int send(const char *message);
	const char *receive();
	void disconnect();
	bool is_connected();

private:
	int _connection_fd;
	unsigned int _connection_length;
	struct sockaddr_in _connection_address;
	bool _connected;
};

