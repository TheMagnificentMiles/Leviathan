#include "Connection.hpp"

void Connection::Connect()
{
	asio::error_code ec;
	asio::io_context context;
	asio::ip::tcp::endpoint endpoint = asio::ip::tcp::endpoint(asio::ip::make_address(UI.connectedIP, ec), 1974);
	asio::ip::tcp::socket socket = asio::ip::tcp::socket(context);
	socket.connect(endpoint, ec);
	if (!ec) { gui.output.push_back("Connected!"); }
	else { gui.output.push_back("Failed to connect to address: \n" + ec.message()); }
}
