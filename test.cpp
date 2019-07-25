/*This function is the communicator module which receive
the client request and pass the merged tiles to the
user.*/

#include "communicator.h"

Communicator::Communicator() {
	//this->tileBuffer1s = tileBuffer1s;
}

bool* Communicator::intializeServer() {
	int res = 0;

	//initialize the varaibles

	this->bindSucess = false;
	this->ListenSocket = false;
	this->acceptSucess = false;


	this->iSendResult = 0;
	this->iResult = 0;

	this->ListenSocket = INVALID_SOCKET;
	this->ListenSocket_2 = INVALID_SOCKET;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"Error at WSAStartup()\n");
		res = -1;
	}

	//listrening for the socket
	assert(res == 0);

	this->ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	this->ListenSocket_2 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (this->ListenSocket == INVALID_SOCKET) {
		wprintf(L"socket function failed with error: %u\n", WSAGetLastError());
		WSACleanup();
		res = -1;
	}
	assert(res == 0);
	if (this->ListenSocket_2 == INVALID_SOCKET) {
		wprintf(L"socket function failed with error: %u\n", WSAGetLastError());
		WSACleanup();
		res = -1;
	}
	assert(res == 0);
	
	//initialize the address and the port
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	service.sin_port = htons(DEFAULT_PORT_1);

	service_2.sin_family = AF_INET;
	service_2.sin_addr.s_addr = inet_addr(IP_ADDRESS);
	service_2.sin_port = htons(DEFAULT_PORT_2);






	iResult = bind(this->ListenSocket, (SOCKADDR*)& service, sizeof(service));
	iResult_2 = bind(this->ListenSocket_2, (SOCKADDR*)& service_2, sizeof(service_2));

	if (iResult == SOCKET_ERROR) {
		wprintf(L"bind failed with error %u\n", WSAGetLastError());
		closesocket(this->ListenSocket);
		WSACleanup();
		res = -1;
	}
	else {
		wprintf(L"bind returned success\n");
		bindSucess = true;
	}
	if (iResult == SOCKET_ERROR) {
		wprintf(L"bind failed with error %u\n", WSAGetLastError());
		closesocket(this->ListenSocket_2);
		WSACleanup();
		res = -1;
	}
	else {
		wprintf(L"bind returned success\n");
		bindSucess_2 = true;
	}
	assert(bindSucess == true);
	assert(bindSucess_2 == true);




	if (listen(this->ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		wprintf(L"listen failed with error: %ld\n", WSAGetLastError());
		closesocket(this->ListenSocket);
		WSACleanup();
	}
	else {
		wprintf(L"Start listening to the client");
		listenSucess = true;
	}
	assert(listenSucess == true);
	wprintf(L"Waiting for client to connect...\n");

	if (listen(this->ListenSocket_2, SOMAXCONN) == SOCKET_ERROR) {
		wprintf(L"listen failed with error: %ld\n", WSAGetLastError());
		closesocket(this->ListenSocket_2);
		WSACleanup();
	}
	else {
		wprintf(L"Start listening to the client");
		listenSucess_2 = true;
	}
	assert(listenSucess_2 == true);
	wprintf(L"Waiting for client to connect...\n");



	bool arry[2] = { listenSucess,listenSucess_2 };

	return arry;
}

int  Communicator::readFrameRequest() {

	char receiveBuffer[DEFAULT_BUFLEN_RECEIVE];

	wprintf(L"Waiting for client to connect...\n");
	this->ClientSocket = accept(this->ListenSocket, NULL, NULL);
	if (this->ClientSocket == INVALID_SOCKET) {
		wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
		closesocket(this->ListenSocket);
		WSACleanup();
	}
/*ssssss*/
	else {
		acceptSucess = true;
		wprintf(L"Client connected.\n");
	}

	wprintf(L"Waiting for client to connect...\n");
	this->ClientSocket_2 = accept(this->ListenSocket_2, NULL, NULL);
	if (this->ClientSocket_2 == INVALID_SOCKET) {
		wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
		closesocket(this->ListenSocket_2);
		WSACleanup();
	}

	else {
		acceptSucess_2 = true;
		wprintf(L"Client_2 connected.\n");
	}




	iResult = recv(this->ClientSocket, receiveBuffer, recvbuflen, 0);
	if (iResult > 0) {
		printf("Bytes received: %d\n", iResult);
		for (int i = 0; i < iResult; i++) {
			printf("%d\n", receiveBuffer[i]);
			recvbuf[i] *= 2;
		}
		readData();


		//sendData();
		memset(recvbuf, 0, sizeof recvbuf);
	}
	else if (iResult == 0)
		printf("Connection closing...\n");
	else {
		printf("recv failed with error: %d\n", WSAGetLastError());
		closesocket(this->ClientSocket);
		WSACleanup();
	}

	return iResult;
}

int Communicator::sendData() {
	iSendResult = send(this->ClientSocket, this->recvbuf, this->iResult, 0);
	if (this->iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(this->ClientSocket);
		WSACleanup();
		//return 1;
	}
	else {
		printf("Bytes sent: %d\n", iSendResult);
	}



	return 0;
	
}

void Communicator::readData()
{
	//AVIOContext *client, const char *in_ur
	AVIOContext *input = NULL;
	uint8_t buf[1024*10];
	int ret, n, reply_code;
	uint8_t *resource = NULL;


	AVIOContext *input2 = NULL;
	uint8_t buf2[1024 * 10];
	int ret2, n2, reply_code2;
	uint8_t *resource2 = NULL;
	/*while ((ret = avio_handshake(client)) > 0) {
		av_opt_get(client, "resource", AV_OPT_SEARCH_CHILDREN, &resource);
		// check for strlen(resource) is necessary, because av_opt_get()
		// may return empty string.
		if (resource && strlen(resource))
			break;
		av_freep(&resource);
	}
	if (ret < 0)
		goto end;
	av_log(client, AV_LOG_TRACE, "resource=%p\n", resource);
	if (resource && resource[0] == '/' && !strcmp((resource + 1), in_uri)) {
		reply_code = 200;
	}
	else {
		reply_code = AVERROR_HTTP_NOT_FOUND;
	}
	if ((ret = av_opt_set_int(client, "reply_code", reply_code, AV_OPT_SEARCH_CHILDREN)) < 0) {
		av_log(client, AV_LOG_ERROR, "Failed to set reply_code: %s.\n", av_err2str(ret));
		goto end;
	}
	av_log(client, AV_LOG_TRACE, "Set reply code to %d\n", reply_code);

	while ((ret = avio_handshake(client)) > 0);

	if (ret < 0)
		goto end;

	fprintf(stderr, "Handshake performed.\n");
	if (reply_code != 200)
		goto end;
	fprintf(stderr, "Opening input file.\n");*/


	const char *in_uri = "E:\\SelfLearning\\Rubiks\\RubiksVideos\\processedVideo_SD\\frame_720_0_0\\frame_000_1.mp4";
	if ((ret = avio_open2(&input, in_uri, AVIO_FLAG_READ, NULL, NULL)) < 0) {

		//av_log(input, AV_LOG_ERROR, "Failed to open input: %s: %s.\n", in_uri,av_err2str(ret));
		cout << "Failed to open input" << endl;
		//goto end;
	}

	const char *in_uri2 = "E:\\SelfLearning\\Rubiks\\RubiksVideos\\processedVideo_SD\\frame_720_0_0\\frame_000_2.mp4";
	if ((ret = avio_open2(&input2, in_uri2, AVIO_FLAG_READ, NULL, NULL)) < 0) {

		//av_log(input, AV_LOG_ERROR, "Failed to open input: %s: %s.\n", in_uri,av_err2str(ret));
		cout << "Failed to open input" << endl;
		//goto end;
	}





	for (;;) {
		n = avio_read(input, buf, sizeof(buf));
		if (n < 0) {
			if (n == AVERROR_EOF)
				break;
			//av_log(input, AV_LOG_ERROR, "Error reading from input: %s.\n",av_err2str(n));
			cout << "Error reading from input" << endl;
			break;
		}
		cout << sizeof(buf) << endl;
		/*avio_write(client, buf, n);
		avio_flush(client);*/

		iSendResult = send(this->ClientSocket, (const char*)buf, n, 0);
		if (this->iSendResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(this->ClientSocket);
			WSACleanup();
			//return 1;
		}
		else {
			printf("Bytes sent: %d\n", iSendResult);
		}
	}
	cout << "done 1" << endl;
	closesocket(ClientSocket);

	for (;;) {
		n2 = avio_read(input2, buf2, sizeof(buf2));
		if (n2 < 0) {
			if (n2 == AVERROR_EOF)
				break;
			//av_log(input, AV_LOG_ERROR, "Error reading from input: %s.\n",av_err2str(n));
			cout << "Error reading from input" << endl;
			break;
		}
		cout << sizeof(buf2) << endl;
		/*avio_write(client, buf, n);
		avio_flush(client);*/

		iSendResult_2 = send(this->ClientSocket_2, (const char*)buf2, n2, 0);
		if (this->iSendResult_2 == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(this->ClientSocket_2);
			WSACleanup();
			//return 1;
		}
		else {
			printf("Bytes sent: %d\n", iSendResult_2);
		}
	}
	cout << "done 2" << endl;
	closesocket(ClientSocket_2);

/*end:
	fprintf(stderr, "Flushing client\n");
	avio_flush(client);
	fprintf(stderr, "Closing client\n");
	avio_close(client);
	fprintf(stderr, "Closing input\n");
	avio_close(input);
	av_freep(&resource);*/
}


int main() {

	Communicator comm;
	comm.intializeServer();
	comm.readFrameRequest();
	
	getchar();
	

}