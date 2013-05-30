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
