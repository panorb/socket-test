#pragma once

#include "socket_address.hpp"


class UDPSocket
{
public:
    ~UDPSocket();
    int Bind(const SocketAddress& in_bind_address);
    int SendTo(const void* in_data, int in_max_length, const SocketAddress& in_to);
    int ReceiveFrom(void* out_buffer, int in_max_length, SocketAddress& out_from);
    int SetNonBlockingMode(bool in_should_be_non_blocking);
private:
    friend class SocketUtil;
    UDPSocket(SOCKET socket) : socket_(socket) {}
    SOCKET socket_;
};
typedef std::shared_ptr<UDPSocket> UDPSocketPtr;
