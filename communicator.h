#pragma once
#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H


#define WIN32_LEAN_AND_MEAN

#include "commomdata.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <Windows.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma warning(disable:4996) 
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_PORT_1 5551
#define DEFAULT_PORT_2 5552
#define IP_ADDRESS "10.130.1.229"

class Communicator {
private:
	/*as the function output parameters*/
	int iSendResult;
	int iResult;

	int iSendResult_2;
	int iResult_2;

	//flags for getting the sucess of each state
	bool bindSucess;
	bool listenSucess;
	bool acceptSucess;

	bool bindSucess_2;
	bool listenSucess_2;
	bool acceptSucess_2;

	char recvbuf[DEFAULT_BUFLEN_RECEIVE];
	int recvbuflen = DEFAULT_BUFLEN_RECEIVE;



	WSADATA wsaData;

	// The socket address to be passed to bind
	sockaddr_in service;
	sockaddr_in service_2;

	/*Buffer to store the data to be sent. ideally this is a
	~7 frames x 4 layers x 4 tiles which has extra 2 capacity to store
	extra 2 tiles data*/
	tileBuffer* tileBuffer1s;


public:
	Communicator();
	bool* intializeServer();
	int readFrameRequest();
	int sendData();
	void readData();

	// the listening socket to be created
	SOCKET ListenSocket;
	SOCKET ListenSocket_2;
	SOCKET ClientSocket;
	SOCKET ClientSocket_2;
};


#endif // !COMMUNICATOR_H

