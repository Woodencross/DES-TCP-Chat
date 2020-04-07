#include"des.h"
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<thread>

#pragma comment(lib,"ws2_32.lib")

//发送子线程
void sendData(SOCKET sClient) {
	string data;
	while (true) {
		getline(cin, data);
		cout << "Client>>>" << data << endl;
		data = encrypt(data);
		send(sClient, data.data(), data.length(), 0);
	}
}

//接收父线程
int main() {
	//初始化
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0) {
		cout << "Initialize WSA failed !" << endl;
		return 0;
	}

	//
	
	SOCKET sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET) {
		cout << "Create socket failed !" << endl;
		return 0;
	}

	sockaddr_in serAddr;
	memset(&serAddr, 0, sizeof(serAddr));
	serAddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serAddr.sin_addr.s_addr);
	serAddr.sin_port = htons(1234);
	if (connect(sClient, (sockaddr*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {
		cout << "Connect error !" << endl;
		closesocket(sClient);
		return 0;
	}

	thread t(sendData, sClient);
	t.detach();


	while (true) {
		char revData[2048];
		int ret = recv(sClient, revData, 2048, 0);
		if (ret > 0) {
			revData[ret] = '\0';
			cout << "Server>>>" << decrypt(revData) << endl;
		}
		else {
			cout << "System: Server closed." << endl;
			break;
		}
	}

	closesocket(sClient);
	WSACleanup();

	system("pause");
	return 0;
}