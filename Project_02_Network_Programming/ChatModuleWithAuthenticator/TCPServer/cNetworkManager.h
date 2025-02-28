#pragma once

#include "cBuffer.h"
#include <AuthProtocolMessages.pb.h>

// Structure representing the packet header.
struct sPacketHeader
{
	uint32_t packetSize;  // Size of the packet.
	uint32_t messageType; // Type of the message.
};

// Structure representing message details.
struct sChatMessage
{
	sPacketHeader header;	// Message header.
	uint32_t messageLength;	// Length of the message.
	std::string message;	// The chat message itself.
};

// Structure representing a chat room with a list of clients.
struct sRoom
{
	std::vector<SOCKET> clients; // List of clients in the room.
};

// This class is created to manage the network operations of the server.
class cNetworkManager
{
private:

	unsigned int requestId = 0;

	std::map<std::string, sRoom> rooms;			// Map of room names as key and the sRoom instance as value.

	std::map<SOCKET, std::string> clientRooms;	// Map of client's socket as key and the client's room name as value.

	std::map<SOCKET, std::string> clientName;	// Map of client's socket as key and the client's name as value.

	std::vector<SOCKET> mClientConnectionList;	// List of connected client sockets.

	std::map<SOCKET, std::string> mClientEmailList;

	std::map<unsigned int, SOCKET> mClientRequestIdList;

	std::map<SOCKET, unsigned int> mClientUserIdList;

public:

	SOCKET authServerSocketNumber = NULL;

	struct addrinfo* info = nullptr;
	struct addrinfo hints;

	struct addrinfo* info_authServer = nullptr;
	struct addrinfo hints_authServer;

	int readCount = 0;

	int InitializeWSA();
	int InitializeAddrInfo(const char* ipaddress, const char* port);
	int InitializeAddrInfoforAuthServer(const char* ipaddress, const char* port);

	int CreateSocket(SOCKET& serverSocket);
	int CreateSocketForAuthServer(SOCKET& authServerSocket);

	int Bind(SOCKET& serverSocket);
	int Listen(SOCKET& serverSocket);
	int ConnectSocket(SOCKET& serverSocket);
	void CleanSocket(SOCKET& serverSocket, PADDRINFOA info);

	void SendMessageToBuffer(sChatMessage& message, cBuffer& buffer, std::string input);
	int SendMessageToClient(sChatMessage& message, cBuffer& buffer, std::string fullMessage, SOCKET clientSocket);
	int SendMessageToAuthServer(std::string messageToBeSent);

	int ReceiveMessageFromClient(SOCKET clientSocket, cBuffer& buffer, int bufSize);
	int ReceiveMessageFromAuthServer(sChatMessage& message, cBuffer& buffer);

	void AddAllClientsToFDSET(FD_SET& socketsReadyForReading);
	int AddNewClientToList(SOCKET& serverSocket, FD_SET& socketsReadyForReading, FD_SET& activeSockets);
	void RemoveClientFromList(SOCKET& clientSocket);

	int LoopThroughClientList(FD_SET& socketsReadyForReading, FD_SET& activeSockets, sChatMessage& message, cBuffer& buffer, int bufSize);
	int BroadcastToAll(SOCKET& clientSocket, bool newClientJoined, cBuffer& buffer, sChatMessage message, std::string userRoom);
};

