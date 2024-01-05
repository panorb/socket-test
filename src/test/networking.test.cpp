#include <catch.hpp>
#include "core/networking.hpp"

// Test creation of sockets
TEST_CASE("Socket creation", "[networking]")
{
	SocketUtil::StaticInit();
	REQUIRE(SocketUtil::GetLastError() == 0);

	SECTION("Create UDPSocket") {
		UDPSocketPtr socket = SocketUtil::CreateUDPSocket(SocketAddressFamily::INET);
		REQUIRE(socket != nullptr);
	}
	
	SECTION("Create TCPSocket") {
		TCPSocketPtr socket = SocketUtil::CreateTCPSocket(SocketAddressFamily::INET);
		REQUIRE(socket != nullptr);
	}
}

TEST_CASE("Address resolving", "[networking]")
{
	SocketUtil::StaticInit();

	SocketAddressPtr address = SocketAddressFactory::CreateIPv4FromString("foundry.yellowkamel.de");
	REQUIRE(address != nullptr);
	REQUIRE(address->GetSize() == sizeof(sockaddr));


	address = SocketAddressFactory::CreateIPv4FromString("thisisnotadomain.pop");
	REQUIRE(address == nullptr);
}
