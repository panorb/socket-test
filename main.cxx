// #include "networking.hpp"
#include "socket_hello_world.cpp"

int main() {
	mainLoop();
//	std::vector<spdlog::sink_ptr> sinks;
//	sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
//	sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt"));
//
//	auto combined_logger = std::make_shared<spdlog::logger>("", begin(sinks), end(sinks));
//	spdlog::set_default_logger(combined_logger);
//
//	spdlog::info("==================================================");
//	spdlog::info("Program is starting...");
//
//	WSADATA wsaData;
//
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//		spdlog::error("WSAStartup() failed.");
//		return 0;
//	}
//
//	spdlog::info("WSAStartup() succeeded.");
//
//	if (WSACleanup() != 0) {
//		spdlog::error("WSACleanup() failed.");
//	}
//	else {
//		spdlog::info("WSACleanup() succeeded.");
//	}
//
//	spdlog::info("Program exited. Goodbye.");
}

