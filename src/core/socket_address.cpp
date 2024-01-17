#include "socket_address.hpp"


// SocketAddress class
SocketAddress::SocketAddress(uint32_t inAddress, uint16_t inPort) {
    GetAsSockAddrIn()->sin_family = AF_INET;
    GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
    GetAsSockAddrIn()->sin_port = htons(inPort);
}

SocketAddress::SocketAddress(const sockaddr& inSockAddr) {
    memcpy(&sock_addr_, &inSockAddr, sizeof(sockaddr));
}

size_t SocketAddress::GetSize() const
{
    return sizeof(sockaddr);
}

sockaddr_in* SocketAddress::GetAsSockAddrIn()
{
    return reinterpret_cast<sockaddr_in*>(&sock_addr_);
}

// SocketAddressFactory helper
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
