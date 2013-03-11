/*
 * framework.h
 *
 * This file contains the interface for the server framework.
 * This includes functions, structures and typedefs.
 */

#pragma once

#include <pthread.h>
#include "listener.h"

// Server callback for each client
typedef int(*connection_callback)(Connection*);

// Struct containing information about a server, used for thread parameters.
// Should not be accessed outside functions, except as a reference for
// function calls.
struct ServerInfo
{
	unsigned int _port;
	connection_callback _callback;
	Listener *_listener;
	pthread_t _thread;
	pthread_mutex_t _lock;
	int _startcode;
	bool _stop;
	ServerInfo(unsigned int port, connection_callback callback) :
		_port(port),
		_callback(callback),
		_listener(NULL),
		_thread(0),
		_startcode(0),
		_stop(false) { }
};

// Struct containing information about a connected client, used for thread parameters.
// Should not be accessed outside functions.
struct ClientInfo
{
	Connection *_connection;
	connection_callback _callback;
	ClientInfo(Connection *connection, connection_callback callback) :
		_connection(connection),
		_callback(callback) { }
};

ServerInfo *run_server(unsigned int port, connection_callback callback);
bool server_has_errors(ServerInfo *server);
void stop_server(ServerInfo *server);
