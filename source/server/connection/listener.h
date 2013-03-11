/*
 * Listener.h
 *
 * This file contains the interface for the listener class
 */

#pragma once

#include "connection.h"

class Listener {
public:
	Listener();
	virtual ~Listener();

public:
	int start(unsigned int port);
	Connection *accept_client(); // This is a blocking call.
	void stop();

private:
	int _socket_fd;
	struct sockaddr_in _server_address;
	bool _stop;
};

