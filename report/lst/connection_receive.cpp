char *Connection::receive()
{
	if (!this->is_connected())
	{
		fprintf(stderr, "ERROR: Attempt to read from socket with no connection\n");
		return NULL;
	}

	// Set up poll structure to listen for IN-data on stream
	pollfd p;
	p.fd = _connection_fd;
	p.events = POLLIN;

	poll(&p, 1, -1); // Wait for data

	char *buffer = (char *)malloc(BUFFER_SIZE); // Chunk buffer
	std::stringstream msg; // Message buffer

	do
	{
		memset((void *)buffer, 0, BUFFER_SIZE);
		int n = read(_connection_fd, buffer, BUFFER_SIZE - 1);
		if (n < 0)
		{
			fprintf(stderr, "ERROR: Failed reading from socket.\n");
			break;
		}
		msg << buffer;
		// See if more data is available, with 100ms timeout,
		// this is necessary for stream (TCP) sockets.
		poll(&p, 1, 100);
	}
	while (p.revents & POLLIN);

	free(buffer);
	std::string s = msg.str();

	char *m = new char[s.size() + 1];
	std::copy(s.begin(), s.end(), m);
	return m;
}
