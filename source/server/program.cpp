
#include "connection/framework.h"
#include "api/api.h"
#include <iostream>



int handle_api(Connection *connection)
{
	const char *request = connection->receive();
	std::string response = (new API())->handle_request(request);
	connection->send(response.c_str());
	return 0;
}





int main (int argc, char *argv[])
{

	
	ServerInfo *server = run_server(2468, handle_api);

	char *a = "derp";

	while (a[0] != 's')
	{
		std::cin >> a;
	}

	stop_server(server);
	
	return 0;
}
