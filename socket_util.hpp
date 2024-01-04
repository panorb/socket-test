#pragma once

#include "udp_socket.hpp"
#include "tcp_socket.hpp"


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
