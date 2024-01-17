#include "socket_util.hpp"


bool SocketUtil::StaticInit()
{
#ifdef WIN32
    WSADATA wsaData;
    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0)
    {
        ReportError("Startup");
        return false;
    }
#endif
    return true;
}

void SocketUtil::ReportError(const std::string& error)
{
    spdlog::error("{}", error);
}

int SocketUtil::GetLastError()
{
#ifdef WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily inFamily)
{
    SOCKET s = socket(inFamily, SOCK_DGRAM, IPPROTO_UDP);
    if (s != INVALID_SOCKET)
    {
        return UDPSocketPtr(new UDPSocket(s));
    }
    else
    {
        ReportError("SocketUtil::CreateUDPSocket");
        return nullptr;
    }
}

TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressFamily inFamily) {
    SOCKET s = socket(inFamily, SOCK_STREAM, IPPROTO_TCP);
    if (s != INVALID_SOCKET)
    {
        return TCPSocketPtr(new TCPSocket(s));
    }
    else {
        ReportError("SocketUtil::CreateTCPSocket");
        return nullptr;
    }
}

fd_set* SocketUtil::FillSetFromVector(fd_set& outSet,
    const std::vector<TCPSocketPtr>* inSockets)
{
    if (inSockets)
    {
        FD_ZERO(&outSet);
        for (const TCPSocketPtr& socket : *inSockets)
        {
            FD_SET(socket->socket_, &outSet);
        }
        return &outSet;
    }
    else
    {
        return nullptr;
    }
}

void SocketUtil::FillVectorFromSet(std::vector<TCPSocketPtr>* outSockets,
    const std::vector<TCPSocketPtr>* inSockets,
    const fd_set& inSet)
{
    if (inSockets && outSockets)
    {
        outSockets->clear();
        for (const TCPSocketPtr& socket : *inSockets)
        {
            if (FD_ISSET(socket->socket_, &inSet))
            {
                outSockets->push_back(socket);
            }
        }
    }
}

int SocketUtil::Select(const std::vector<TCPSocketPtr>* inReadSet,
    std::vector<TCPSocketPtr>* outReadSet,
    const std::vector<TCPSocketPtr>* inWriteSet,
    std::vector<TCPSocketPtr>* outWriteSet,
    const std::vector<TCPSocketPtr>* inExceptSet,
    std::vector<TCPSocketPtr>* outExceptSet)
{
    //build up some sets from our vectors
    fd_set read, write, except;

    fd_set* readPtr = FillSetFromVector(read, inReadSet);
    fd_set* writePtr = FillSetFromVector(write, inWriteSet);
    fd_set* exceptPtr = FillSetFromVector(except, inExceptSet);

    int toRet = select(0, readPtr, writePtr, exceptPtr, nullptr);

    if (toRet > 0)
    {
        FillVectorFromSet(outReadSet, inReadSet, read);
        FillVectorFromSet(outWriteSet, inWriteSet, write);
        FillVectorFromSet(outExceptSet, inExceptSet, except);
    }
    return toRet;
}
