
#include "connection/framework.h"
#include "api/api.h"
#include <iostream>



int handle_api(Connection *connection)
{
	char *request = connection->receive();
	std::cout << request << std::endl;
	std::string response = (new API())->handle_request(request);

	delete[] request;
	std::cout << response.c_str() << std::endl;
	connection->send(response.c_str());
	connection->disconnect();
	return 0;
}





int main (int argc, char *argv[])
{
	ServerInfo *server = run_server(2468, handle_api);

	char *a = (char *)malloc(64);

	while (a[0] != 's')
	{
		std::cin >> a;
	}

	free(a);

	stop_server(server);
	
	return 0;
}
