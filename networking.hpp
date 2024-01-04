// External dependencies
#include <spdlog/spdlog.h>
// Standard library
#include <cstdint>
#include <string>
#include <memory>
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
	SocketAddress(uint32_t inAddress, uint16_t inPort);;

	SocketAddress(const sockaddr& inSockAddr);;

	size_t GetSize() const;

private:
    friend class UDPSocket;
    friend class TCPSocket;
	sockaddr mSockAddr;
	sockaddr_in* GetAsSockAddrIn();
};

typedef std::shared_ptr<SocketAddress> SocketAddressPtr;

class SocketAddressFactory {
public:
	static SocketAddressPtr CreateIPv4FromString(const std::string& inString);
};

class UDPSocket
{
public:
    ~UDPSocket();
    int Bind(const SocketAddress& inToAddress);
    int SendTo(const void* inData, int inLen, const SocketAddress& inTo);
    int ReceiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom);
    int SetNonBlockingMode(bool inShouldBeNonBlocking);
private:
    friend class SocketUtil;
    UDPSocket(SOCKET inSocket) : mSocket(inSocket) {}
    SOCKET mSocket;
};
typedef std::shared_ptr<UDPSocket> UDPSocketPtr;

class TCPSocket
{
public:
    ~TCPSocket();
    int                     Connect(const SocketAddress& inAddress);
    int                     Bind(const SocketAddress& inToAddress);
    int                     Listen(int inBackLog = 32);
    std::shared_ptr< TCPSocket > Accept(SocketAddress& inFromAddress);
    int                     Send(const void* inData, int inLen);
    int                     Receive(void* inBuffer, int inLen);
private:
    friend class SocketUtil;
    TCPSocket(SOCKET inSocket) : mSocket(inSocket) {}
    SOCKET mSocket;
};
typedef std::shared_ptr<TCPSocket> TCPSocketPtr;

class SocketUtil {
public:
    static bool StaticInit();
    static void ReportError(const std::string& error);
    static int GetLastError();
    static UDPSocketPtr CreateUDPSocket(SocketAddressFamily inFamily);
    static TCPSocketPtr CreateTCPSocket(SocketAddressFamily inFamily);
    static fd_set* FillSetFromVector(fd_set& outSet,
        const std::vector<TCPSocketPtr>*
        inSockets);
    static void FillVectorFromSet(std::vector<TCPSocketPtr>* outSockets,
        const std::vector<TCPSocketPtr>*
        inSockets,
        const fd_set& inSet);
    static int Select(const std::vector<TCPSocketPtr>* inReadSet,
        std::vector<TCPSocketPtr>* outReadSet,
        const std::vector<TCPSocketPtr>* inWriteSet,
        std::vector<TCPSocketPtr>* outWriteSet,
        const std::vector<TCPSocketPtr>* inExceptSet,
        std::vector<TCPSocketPtr>* outExceptSet);
};
