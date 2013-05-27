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
