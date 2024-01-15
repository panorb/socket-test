#include "udp_socket.hpp"
#include "socket_util.hpp"


UDPSocket::~UDPSocket()
{
    closesocket(mSocket);
}

int UDPSocket::Bind(const SocketAddress& inBindAddress)
{
    int err = bind(mSocket, &inBindAddress.mSockAddr, static_cast<int>(inBindAddress.GetSize()));
    // int err = bind(mSocket, &inBindAddress.mSockAddr,
    //    inBindAddress.GetSize());
    if (err != 0)
    {
        SocketUtil::ReportError("UDPSocket::Bind");
        return SocketUtil::GetLastError();
    }
    return NO_ERROR;
}


int UDPSocket::SendTo(const void* inData, int inLen, const SocketAddress& inTo)
{
    int byteSentCount = sendto(mSocket,
        static_cast<const char*>(inData),
        inLen,
        0, &inTo.mSockAddr,
        static_cast<int>(inTo.GetSize()));
    if (byteSentCount >= 0)
    {
        return byteSentCount;
    }
    else
    {
        //return error as negative number
        SocketUtil::ReportError("UDPSocket::SendTo");
        return -SocketUtil::GetLastError();
    }
}

int UDPSocket::ReceiveFrom(void* inBuffer, int inMaxLength, SocketAddress& outFromAddress)
{
    int fromLength = static_cast<int>(outFromAddress.GetSize());
    int readByteCount = recvfrom(mSocket,
        static_cast<char*>(inBuffer),
        inMaxLength,
        0, &outFromAddress.mSockAddr,
        &fromLength);
    if (readByteCount >= 0)
    {
        return readByteCount;
    }
    else
    {
        SocketUtil::ReportError("UDPSocket::ReceiveFrom");
        return -SocketUtil::GetLastError();
    }
}

int UDPSocket::SetNonBlockingMode(bool inShouldBeNonBlocking)
{
#if _WIN32
    u_long arg = inShouldBeNonBlocking ? 1 : 0;
    int result = ioctlsocket(mSocket, FIONBIO, &arg);
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
