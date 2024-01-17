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
	SocketAddress(uint32_t address, uint16_t port);;

	SocketAddress(const sockaddr& sock_addr);;

	size_t GetSize() const;

private:
	friend class UDPSocket;
	friend class TCPSocket;
	sockaddr sock_addr_;
	sockaddr_in* GetAsSockAddrIn();
};

typedef std::shared_ptr<SocketAddress> SocketAddressPtr;

class SocketAddressFactory {
public:
	static SocketAddressPtr CreateIPv4FromString(const std::string& resolve_string);
};
