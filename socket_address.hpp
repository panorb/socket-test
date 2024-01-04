#pragma once

// External dependencies
#include <spdlog/spdlog.h>
// Standard library
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
// Networking libaries
#include <WinSock2.h>
#include <WS2tcpip.h>


enum SocketAddressFamily
{
	INET = AF_INET,
	INET6 = AF_INET6
};

class SocketAddress {
public:
	SocketAddress(uint32_t inAddress, uint16_t inPort);;

	SocketAddress(const sockaddr& inSockAddr);;

	size_t GetSize() const;

private:
	friend class UDPSocket;
	friend class TCPSocket;
	sockaddr mSockAddr;
	sockaddr_in* GetAsSockAddrIn();
};

typedef std::shared_ptr<SocketAddress> SocketAddressPtr;

class SocketAddressFactory {
public:
	static SocketAddressPtr CreateIPv4FromString(const std::string& inString);
};
