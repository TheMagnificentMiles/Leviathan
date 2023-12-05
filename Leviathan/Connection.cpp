#include "Connection.hpp"

void Connection::Connect()
{
    //maybe stuffs
}

void Connection::SendError(std::string message)
{
	Send(2, &message);
}

void Connection::SendWarning(std::string message)
{
	Send(1, &message);
}

void Connection::SendTelemetry(std::string key, std::string value)
{
    std::string message = key + "!" + value;
	Send(3, &message);
}

void Connection::Send(int header, void * message)
{
    std::string msgLength = std::to_string(sizeof(message));
    msgLength.insert(0, 32-msgLength.size(), ' ');
    auto lenSent = socket.send_to(asio::buffer(msgLength, 32), remote_endpoint, 0);
    std::cout << "Sent length --- " << lenSent << "\n";

    std::string msgHeader = std::to_string(header);
    msgHeader.insert(0, 32-msgHeader.size(), ' ');
    auto headerSent = socket.send_to(asio::buffer(msgHeader, 32), remote_endpoint, 0);
    std::cout << "Sent header --- " << headerSent << "\n";

    auto msgSent = socket.send_to(asio::buffer(message, sizeof(message)), remote_endpoint, 0);
    std::cout << "Sent message --- " << msgSent << "\n";
}

void Connection::HandleReceive(std::size_t bytes_received){
    int length;
    int header;
    std::cout << "Number Received = " << (numMessages+1) << std::endl;
    if(numMessages==0){
        length = stoi(std::string(recv_buffer.data(), bytes_received));
        numMessages++;
    }
    else if(numMessages==1){
        header = stoi(std::string(recv_buffer.data(), bytes_received));
        numMessages++;
    }
    else if(numMessages == 2){
        std::cout << "Received all three messages" << std::endl;

        std::string message = std::string(recv_buffer.data());

        std::cout << "Length:" << length << std::endl;
        std::cout << "Header:" << header << std::endl;
        std::cout << "Message:" << message << std::endl;

        numMessages=0;

        StartReceiving();
    }
}

void Connection::HandleHandshake(){
    asio::error_code error;
    socket.receive_from(asio::buffer(recv_buffer), remote_endpoint, 0, error);
    if(recv_buffer.data() != NULL){
        if(std::string(recv_buffer.data()) != "0110"){
            std::cout << "[WARNING] Handshake with client failed" << std::endl;
        }else{
            connDetails.connectedIP = remote_endpoint.address().to_string();
            connDetails.connectedPort = remote_endpoint.port();
            connDetails.connectionStatus = "Connected";
            UI::Get()->setConnectionDetails(connDetails);
            StartReceiving();
        }
    }else{
        std::cout << "Error Code for receiving: " << error.message() << std::endl;
    }
}

Connection::~Connection()
{
}

Connection* Connection::Get()
{
	if (connection == nullptr)
		connection = new Connection();

	return connection;
}
