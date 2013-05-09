#define BOOST_TEST_MODULE api
#include <boost/test/included/unit_test.hpp>

#include "api.h"

BOOST_AUTO_TEST_SUITE(api_calls)

BOOST_AUTO_TEST_CASE(api_structure)
{
	Json::Value check;
	Json::Reader reader;

	const char *no_key = "{}";
	reader.parse(API().handle_request(no_key), check);
	BOOST_CHECK(check["errors"].size() > 0);

	const char *no_action = "{\"auth\":{}, \"data\":null}";
	reader.parse(API().handle_request(no_action), check);
	BOOST_CHECK(check["errors"].size() > 0);

	const char *no_data = "{\"auth\":{}, \"action\":null}";
	reader.parse(API().handle_request(no_data), check);
	BOOST_CHECK(check["errors"].size() > 0);

	const char *no_auth = "{\"data\":null, \"action\":null}";
	reader.parse(API().handle_request(no_data), check);
	BOOST_CHECK(check["errors"].size() > 0);

	const char *structure = "{\"data\":null, \"action\":null, \"auth\":{\"username\":\"\", \"password\":\"\"}}";
	reader.parse(API().handle_request(no_data), check);
	BOOST_CHECK(strcmp(check["status"].asCString(), STATUS_STRUCTURE) != 0);

}

BOOST_AUTO_TEST_CASE(api_null)
{

}

BOOST_AUTO_TEST_CASE(api_read)
{

}

BOOST_AUTO_TEST_CASE(api_update)
{

}

BOOST_AUTO_TEST_CASE(api_delete)
{

}

BOOST_AUTO_TEST_CASE(api_create)
{

}

BOOST_AUTO_TEST_SUITE_END()
