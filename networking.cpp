#include "networking.hpp"

// SocketUtil helper
/////////////////////////////////////////////////
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
    return nullptr;
}

fd_set* SocketUtil::FillSetFromVector(fd_set& outSet,
    const std::vector<TCPSocketPtr>* inSockets)
{
    if (inSockets)
    {
        FD_ZERO(&outSet);
        for (const TCPSocketPtr& socket : *inSockets)
        {
            FD_SET(socket->mSocket, &outSet);
        }
        return &outSet;
    }
    else
    {
        return nullptr;
    }
}

void SocketUtil::FillVectorFromSet(std::vector<TCPSocketPtr>* outSockets,
    const std::vector<TCPSocketPtr>*
    inSockets,
    const fd_set& inSet)
{
    if (inSockets && outSockets)
    {
        outSockets->clear();
        for (const TCPSocketPtr& socket : *inSockets)
        {
            if (FD_ISSET(socket->mSocket, &inSet))
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

// SocketAdress class
/////////////////////////////////////////////////
SocketAddress::SocketAddress(uint32_t inAddress, uint16_t inPort) {
	GetAsSockAddrIn()->sin_family = AF_INET;
	GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
	GetAsSockAddrIn()->sin_port = htons(inPort);
}

SocketAddress::SocketAddress(const sockaddr& inSockAddr) {
	memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
}

size_t SocketAddress::GetSize() const
{
	return sizeof( sockaddr );
}

sockaddr_in* SocketAddress::GetAsSockAddrIn()
{
	return reinterpret_cast<sockaddr_in*>(&mSockAddr);
}

// SocketAddressFactory helper
/////////////////////////////////////////////////
SocketAddressPtr SocketAddressFactory::CreateIPv4FromString(const std::string& inString)
{
    auto pos = inString.find_last_of(':');
    std::string host, service;
    if (pos != std::string::npos)
    {
        host = inString.substr(0, pos);
        service = inString.substr(pos + 1);
    }
    else
    {
        host = inString;
        //use default port...
        service = "0";
    }
    addrinfo hint;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;

    addrinfo* result;
    int error = getaddrinfo(host.c_str(), service.c_str(),
        &hint, &result);
    addrinfo* initResult = result;
    if (error != 0)
    {
        if (result != nullptr) {
			freeaddrinfo(result);
		}
        return nullptr;
    }

    while (!result->ai_addr && result->ai_next)
    {
        result = result->ai_next;
    }
    if (!result->ai_addr)
    {
        freeaddrinfo(initResult);
        return nullptr;
    }
    auto toRet = std::make_shared< SocketAddress >(*result->ai_addr);

    freeaddrinfo(initResult);

    return toRet;
}

// UDPSocket class
/////////////////////////////////////////////////
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

