#pragma once

#include "socket_address.hpp"

class TCPSocket
{
public:
	~TCPSocket();
	int Connect(const SocketAddress& in_address);
	int Bind(const SocketAddress& in_to_address);
	int Listen(int in_backlog = 32);
	std::shared_ptr< TCPSocket > Accept(SocketAddress& in_from_address);
	int Send(const void* in_data, int in_len);
	int Receive(void* out_buffer, int in_len);
private:
	friend class SocketUtil;
	TCPSocket(SOCKET socket) : socket_(socket) {}
	SOCKET socket_;
};
typedef std::shared_ptr<TCPSocket> TCPSocketPtr;
