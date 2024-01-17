#pragma once

#include "udp_socket.hpp"
#include "tcp_socket.hpp"


class SocketUtil {
public:
    static bool StaticInit();
    static void ReportError(const std::string& error);
    static int GetLastError();
    static UDPSocketPtr CreateUDPSocket(SocketAddressFamily family);
    static TCPSocketPtr CreateTCPSocket(SocketAddressFamily family);
    static fd_set* FillSetFromVector(fd_set& out_set,
        const std::vector<TCPSocketPtr>*
        in_sockets);
    static void FillVectorFromSet(std::vector<TCPSocketPtr>* out_sockets,
        const std::vector<TCPSocketPtr>* in_sockets,
        const fd_set& in_set);
    static int Select(const std::vector<TCPSocketPtr>* in_read_set,
        std::vector<TCPSocketPtr>* out_read_set,
        const std::vector<TCPSocketPtr>* in_write_set,
        std::vector<TCPSocketPtr>* out_write_set,
        const std::vector<TCPSocketPtr>* in_except_set,
        std::vector<TCPSocketPtr>* out_except_set);
};
