#include "Network.h"
#include "Client.h"
#include <iostream>

Connection::Connection(asio::io_context& IoContext)
	: socket(IoContext)
	, started(false)
	, packet("")
{

}

// Add callback to delete on disconnection
Connection::~Connection()
{
	std::cout << "Connection::~Connection" << std::endl;
}

std::shared_ptr<Connection> Connection::create(asio::io_context& IoContext)
{
	return std::shared_ptr<Connection>(new Connection(IoContext));
}

asio::ip::tcp::socket& Connection::getSocket()
{
	return socket;
}

void Connection::start()
{
	if (!started)
	{
		startReading();
		started = true;
	}
}

void Connection::sendPacket(std::string Packet)
{
	std::string *tmp = new std::string(Packet);
	socket.async_send(asio::buffer(*tmp), [self = shared_from_this(), tmp](const asio::error_code& error, size_t bytes)
	{
		self->handle_write(error, bytes);
		if (tmp != nullptr)
			delete tmp;
	});
}

void Connection::startReading()
{
	auto self = shared_from_this();
	socket.async_receive(asio::buffer(packet), [self = shared_from_this()](const asio::error_code& error, size_t bytes)
		{ self->handle_read(error, bytes); });
}

void Connection::handle_write(const asio::error_code& error, size_t bytes)
{
	if (error)
	{
		std::cerr << "Error: Connection::handle_write(" << error.message() << ", " << bytes << ")" << std::endl;
		disconnect();
	}
}

void Connection::handle_read(const asio::error_code& error, size_t bytes)
{
	if (error)
	{
		std::cerr << "Error: Connection::handle_read(" << error.message() << ", " << bytes << ")" << std::endl;
		disconnect();
	}
	else
	{
		if (bytes > 0)
		{
			std::string loginPacket = Cryptography::LoginDecrypt(std::string(packet, packet + bytes));
			if (loginPacket.back() == '\n')
				loginPacket.erase(loginPacket.end() - 1);
			Client client(loginPacket, socket.remote_endpoint().address().to_string(), false); // change it for handling maintenance
			std::string s = client.getResponse();
			sendPacket(Cryptography::LoginEncrypt(s));
			disconnect();
		}
		else
		{
			std::cerr << "Connection::handle_read(" << error.message() << ", " << bytes << ")" << std::endl;
		}
	}
}

void Connection::disconnect()
{
	socket.shutdown(asio::ip::tcp::socket::shutdown_both);
	socket.close();
	// send signal to remove this object from Server::connections vector
}



Network::Network(asio::io_context& IoContext, unsigned short Port)
	: ioContext(IoContext)
	, acceptor(ioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), Port))
	, started(false)
{

}

void Network::startServer()
{
	if (!started)
	{
		startAccept();
		started = true;
	}
}

void Network::startAccept()
{
	std::shared_ptr<Connection> connection = Connection::create(ioContext);
	connections.push_back(connection);
	asio::error_code er;
	acceptor.async_accept(connection->getSocket(), std::bind(&Network::handle_accept, this, connection, er));
	std::cout << "Server successfully started" << std::endl;
}

void Network::handle_accept(std::shared_ptr<Connection> connection, const asio::error_code& error)
{
	if(error)
		std::cerr << "Error: Network::handle_accept(" << connection->getSocket().remote_endpoint().address().to_string()
		<< ", " << error.message() << ")" << std::endl;
	else
		connection->start();
	startAccept();
}
