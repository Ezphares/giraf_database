
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
	char *api = "e";




	while (api[0] != 's')
	{
		//std::cin.getline(api, 1023);
		char const *out = API().handle_request("{\"data\":{\"type\":\"profile\", \"view\":\"list\", \"ids\":null}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}");
		std::cout << out  << std::endl;
		break;
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
