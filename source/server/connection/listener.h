/*
 * Listener.h
 *
 * TODO This file contains...
 */

#pragma once

#include "connection.h"

class Listener {
public:
	Listener();
	virtual ~Listener();

public:
	int start(unsigned int port);
	Connection *accept_client();
	void stop();

private:
	int _socket_fd;
	struct sockaddr_in _server_address;
};

