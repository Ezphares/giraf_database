
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
	char *api = "e";




	while (api[0] != 's')
	{
		//std::cin.getline(api, 1023);
		std::string out = API().handle_request("{\"data\":{\"type\":\"pictogram\", \"view\":\"list\", \"ids\":null}, \"action\":\"read\", \"auth\":{\"username\":\"john\", \"password\":\"123456\"}}");
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
