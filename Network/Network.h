#pragma once

#include "Cryptography/Cryptography.h"
#include <asio.hpp>

class Connection : public std::enable_shared_from_this<Connection>
{
public:
	~Connection();
	static std::shared_ptr<Connection> create(asio::io_context& IoContext);
	asio::ip::tcp::socket& getSocket();
	void start();

private:
	Connection(asio::io_context& IoContext);
	void startReading();
	void sendPacket(std::string Packet);
	void handle_read(const asio::error_code& error, size_t bytes);
	void handle_write(const asio::error_code& error, size_t bytes);
	void disconnect();

	asio::ip::tcp::socket socket;
	char packet[4096];
	bool started;
};

class Network
{
public:
	Network(asio::io_context& IoContext, unsigned short Port);
	void startServer();

private:
	void startAccept();
	void handle_accept(std::shared_ptr<Connection> connection, const asio::error_code& error);

	asio::io_context& ioContext;
	asio::ip::tcp::acceptor acceptor;
	bool started;
	std::vector<std::shared_ptr<Connection>> connections;
};
