#include "udp_socket.hpp"
#include "socket_util.hpp"


UDPSocket::~UDPSocket()
{
    closesocket(socket_);
}

int UDPSocket::Bind(const SocketAddress& in_bind_address)
{
    int err = bind(socket_, &in_bind_address.sock_addr_, static_cast<int>(in_bind_address.GetSize()));

    if (err != 0)
    {
        SocketUtil::ReportError("UDPSocket::Bind");
        return SocketUtil::GetLastError();
    }
    return NO_ERROR;
}

int UDPSocket::SendTo(const void* in_data, int in_max_length, const SocketAddress& in_to)
{
    int byte_sent_count = sendto(socket_,
        static_cast<const char*>(in_data),
        in_max_length,
        0, &in_to.sock_addr_,
        static_cast<int>(in_to.GetSize()));
    if (byte_sent_count >= 0)
    {
        return byte_sent_count;
    }
    else
    {
        //return error as negative number
        SocketUtil::ReportError("UDPSocket::SendTo");
        return -SocketUtil::GetLastError();
    }
}

int UDPSocket::ReceiveFrom(void* in_buffer, int in_max_length, SocketAddress& out_from_address)
{
    int from_length = static_cast<int>(out_from_address.GetSize());
    int read_byte_count = recvfrom(socket_,
        static_cast<char*>(in_buffer),
        in_max_length,
        0, &out_from_address.sock_addr_,
        &from_length);
    if (read_byte_count >= 0)
    {
        return read_byte_count;
    }
    else
    {
        SocketUtil::ReportError("UDPSocket::ReceiveFrom");
        return -SocketUtil::GetLastError();
    }
}

int UDPSocket::SetNonBlockingMode(bool in_should_be_non_blocking)
{
#if _WIN32
    u_long arg = in_should_be_non_blocking ? 1 : 0;
    int result = ioctlsocket(socket_, FIONBIO, &arg);
#else
    int flags = fcntl(mSocket, F_GETFL, 0);
    flags = inShouldBeNonBlocking ?
        (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
    fcntl(mSocket, F_SETFL, flags);
#endif

    if (result == SOCKET_ERROR)
    {
        SocketUtil::ReportError("UDPSocket::SetNonBlockingMode");
        return SocketUtil::GetLastError();
    }
    else
    {
        return NO_ERROR;
    }
}
