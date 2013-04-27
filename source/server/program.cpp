
#include "connection/framework.h"
#include "api/api.h"
#include <iostream>



int handle_api(Connection *connection)
{
	const char *request = connection->receive();
	const char *response = (new API())->handle_request(request);
	connection->send(response);
	return 0;
}





int main (int argc, char *argv[])
{
	char api[1024];

	while (api[0] != 's')
	{
		std::cin.getline(api, 1023);
		std::cout << API().handle_request(api) << std::endl;
	}




	/*
	ServerInfo *server = run_server(666, handle_api);

	char *a = "";

	while (a[0] != 's')
	{
		std::cin >> a;
	}

	stop_server(server);
	*/

	return 0;
}
