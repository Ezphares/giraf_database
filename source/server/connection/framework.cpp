/*
 * framework.cpp
 *
 * This file contains the implementation of the server framework
 */

#include "framework.h"

// The thread start for a connected client
void *client_runner(void *param)
{
	ClientInfo *client = (ClientInfo *)param;
	// Start the callback
	client->_callback(client->_connection);
	delete client;

	pthread_exit(NULL);
	return NULL;
}

// The thread start for a server
void *server_runner(void *param)
{
	fprintf(stderr, "Running server\n");
	ServerInfo *info = (ServerInfo *)param;

	// Initialize the listener
	info->_listener = new Listener();
	info->_startcode = info->_listener->start(info->_port);

	// Unlock calling thread now that the error code is ready
	pthread_mutex_unlock(&(info->_lock));

	if (server_has_errors(info))
		pthread_exit(NULL);

	// Accept clients until the server is stopped
	while (!info->_stop)
	{
		// Attempt to accept a connection
		Connection *connection = info->_listener->accept_client();
		if (connection == NULL)
			continue;

		if (info->_stop)
		{
			// Do not handle connections after stop signal
			connection->disconnect();
			break;
		}

		// Attempt to start a thread for the client
		// TODO: Implement a "threadpool" to prevent overflow?
		ClientInfo *client = new ClientInfo(connection, info->_callback);
		pthread_t client_thread;
		if (pthread_create(&client_thread, NULL, client_runner, (void *)client) != 0)
		{
			fprintf(stderr, "ERROR: Failed to start client thread\n");
			continue;
		}
		pthread_detach(client_thread);
	}

	fprintf(stderr, "Stopping server.\n");
	info->_listener->stop();
	pthread_exit(NULL);
	return NULL;
}

ServerInfo *run_server(unsigned int port, connection_callback callback)
{
	ServerInfo *param = new ServerInfo(port, callback);
	pthread_mutex_init(&(param->_lock), NULL);
	if (pthread_mutex_lock(&(param->_lock)) != 0)
	{
		fprintf(stderr, "ERROR: Unexpected mutex error.\n");
		return param;
	}

	// Start the server on a separate thread
	if (pthread_create(&(param->_thread), NULL, server_runner, (void *)param) != 0)
	{
		fprintf(stderr, "ERROR: Failed to start client thread.\n");
		pthread_mutex_unlock(&(param->_lock));
		return param;
	}
	pthread_mutex_lock(&(param->_lock)); // Wait for success or error on Listener
	pthread_mutex_unlock(&(param->_lock));

	return param;
}

bool server_has_errors(ServerInfo *server)
{
	return (server->_startcode != 0);
}

void stop_server(ServerInfo *server)
{
	server->_stop = true;
	if (!server_has_errors(server))
	{
		// Force a dummy connection to close socket
		Connection dummy;
		dummy.connect_to_host("127.0.0.1", server->_port);
		dummy.disconnect();
	}
	pthread_join(server->_thread, NULL);
}

