#include <fstream>
#include <iostream>
#include <functional>
#include <ctime>
#include <chrono>
#include <string>

#include "Config.h"

#include "../../include/dbcppp/Network.h"
#include <boost/test/unit_test.hpp>
#include <boost/regex.hpp>

namespace utf = boost::unit_test;

BOOST_AUTO_TEST_CASE(DBCParsingCommentRegularExpression)
{
	using namespace dbcppp;
	BOOST_TEST_MESSAGE("DBCParsingCommentError");
	const boost::regex e{ "^//[ 0-9a-zA-Z=-]+$", boost::regex_constants::perl };
	
	//std::ifstream care_application(Core_Application_Message_EQ5_protocol);
	//auto net1 = dbcppp::Network::fromDBC(care_application);

	std::ifstream DV_init(Core_Application_Message_EQ5_protocol);
	std::string str((std::istreambuf_iterator<char>(DV_init)), std::istreambuf_iterator<char>());
	
	str = boost::regex_replace(str, e, "");

	//std::cout << str << std::endl;
	/*
	auto net2 = dbcppp::Network::fromDBC(DV_init);

	net2->forEachMessage([&](const dbcppp::Message &s)
	{
		std::cout << s.getId() << std::endl;
	});
	*/
}
BOOST_AUTO_TEST_CASE(DBCParsingCommentE)
{
	using namespace dbcppp;
	BOOST_TEST_MESSAGE("DBCParsingCommentError");
	
	std::ifstream DV_init(Core_DV_Init);
	
	auto net2 = dbcppp::Network::fromDBC(DV_init);
	
	net2->forEachMessage([&](const dbcppp::Message &s)
	{
		std::cout << s.getId() << std::endl;
	});
	
}