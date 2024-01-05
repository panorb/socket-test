#include "tcp_socket.hpp"
#include "socket_util.hpp"


TCPSocket::~TCPSocket()
{
}

int TCPSocket::Connect(const SocketAddress& inAddress)
{
    int err = connect(mSocket, &inAddress.mSockAddr, static_cast<int>(inAddress.GetSize()));
    if (err < 0)
    {
        SocketUtil::ReportError("TCPSocket::Connect");
        return -SocketUtil::GetLastError();
    }
    return NO_ERROR;
}

int TCPSocket::Bind(const SocketAddress& inToAddress)
{
    return 0;
}

int TCPSocket::Listen(int inBackLog)
{
    int err = listen(mSocket, inBackLog);
    if (err < 0)
    {
        SocketUtil::ReportError("TCPSocket::Listen");
        return -SocketUtil::GetLastError();
    }
    return NO_ERROR;
}

TCPSocketPtr TCPSocket::Accept(SocketAddress& inFromAddress)
{
    int length = static_cast<int>(inFromAddress.GetSize());
    SOCKET newSocket = accept(mSocket, &inFromAddress.mSockAddr, &length);

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

int TCPSocket::Send(const void* inData, int inLen)
{
    int bytesSentCount = send(mSocket,
        static_cast<const char*>(inData),
        inLen, 0);
    if (bytesSentCount < 0)
    {
        SocketUtil::ReportError("TCPSocket::Send");
        return -SocketUtil::GetLastError();
    }
    return bytesSentCount;
}

int TCPSocket::Receive(void* inData, int inLen)
{
    int bytesReceivedCount = recv(mSocket,
        static_cast<char*>(inData), inLen, 0);
    if (bytesReceivedCount < 0)
    {
        SocketUtil::ReportError("TCPSocket::Receive");
        return -SocketUtil::GetLastError();
    }
    return bytesReceivedCount;
}
