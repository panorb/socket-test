#include "tcp_socket.hpp"
#include "socket_util.hpp"


TCPSocket::~TCPSocket()
{
}

int TCPSocket::Connect(const SocketAddress& in_address)
{
    int err = connect(socket_, &in_address.sock_addr_, static_cast<int>(in_address.GetSize()));
    if (err < 0)
    {
        SocketUtil::ReportError("TCPSocket::Connect");
        return -SocketUtil::GetLastError();
    }
    return NO_ERROR;
}

int TCPSocket::Bind(const SocketAddress& in_to_address)
{
    return 0;
}

int TCPSocket::Listen(int in_backlog)
{
    int err = listen(socket_, in_backlog);
    if (err < 0)
    {
        SocketUtil::ReportError("TCPSocket::Listen");
        return -SocketUtil::GetLastError();
    }
    return NO_ERROR;
}

TCPSocketPtr TCPSocket::Accept(SocketAddress& in_from_address)
{
    int length = static_cast<int>(in_from_address.GetSize());
    SOCKET newSocket = accept(socket_, &in_from_address.sock_addr_, &length);

    if (newSocket != INVALID_SOCKET)
    {
        return TCPSocketPtr(new TCPSocket(newSocket));
    }
    else
    {
        SocketUtil::ReportError("TCPSocket::Accept");
        return nullptr;
    }
}

int TCPSocket::Send(const void* in_data, int in_len)
{
    int bytesSentCount = send(socket_,
        static_cast<const char*>(in_data),
        in_len, 0);
    if (bytesSentCount < 0)
    {
        SocketUtil::ReportError("TCPSocket::Send");
        return -SocketUtil::GetLastError();
    }
    return bytesSentCount;
}

int TCPSocket::Receive(void* out_data, int in_len)
{
    int bytesReceivedCount = recv(socket_,
        static_cast<char*>(out_data), in_len, 0);
    if (bytesReceivedCount < 0)
    {
        SocketUtil::ReportError("TCPSocket::Receive");
        return -SocketUtil::GetLastError();
    }
    return bytesReceivedCount;
}
