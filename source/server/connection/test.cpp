#define BOOST_TEST_MODULE connection
#include <boost/test/included/unit_test.hpp>

#include "framework.h"

BOOST_AUTO_TEST_SUITE(connection_lib)

BOOST_AUTO_TEST_CASE( base )
{
	BOOST_CHECK( BUFFER_SIZE >= 2 );
}

BOOST_AUTO_TEST_CASE( classes )
{
	Connection c;
	BOOST_CHECK_EQUAL( c.is_connected(), false ); // Initialization
	BOOST_CHECK_EQUAL( c.connect_to_host("127.0.0.1", 1238), 1); // Connection should fail
	BOOST_CHECK_EQUAL( c.send("Hello World!"), 1); // Sending should fail, error should be printed
	Listener l;
	BOOST_CHECK_EQUAL( l.start(1238), 0 ); // Listening should succeed
	BOOST_CHECK_EQUAL( c.connect_to_host("127.0.0.1", 1238), 0); // Connection should succeed
	Connection *c2 = l.accept_client();
	BOOST_CHECK_EQUAL( c.send("Hello World!"), 0); // Sending should succeed
	BOOST_CHECK_EQUAL( strcmp("Hello World!", c2->receive()), 0); // Should be same message
	c2->disconnect();
	c.disconnect();
	BOOST_CHECK_EQUAL( c.is_connected(), false ); // Disconnected
}

int test(Connection *c)
{
	c->send("Hello World!");
	c->receive();
	c->disconnect();
	return 0;
}

BOOST_AUTO_TEST_CASE( framework )
{

	ServerInfo *i = run_server(1239, test);
	BOOST_CHECK_EQUAL(server_has_errors(i), false); // No errors should be detected
	Connection c;
	BOOST_CHECK_EQUAL( c.connect_to_host("127.0.0.1", 1239), 0); // Connection should succeed
	BOOST_CHECK_EQUAL( strcmp("Hello World!", c.receive()), 0); // Should be same message
	BOOST_CHECK_EQUAL( c.send("Hello World!"), 0); // Sending should succeed
	c.disconnect();
	stop_server(i);
}

BOOST_AUTO_TEST_SUITE_END()
