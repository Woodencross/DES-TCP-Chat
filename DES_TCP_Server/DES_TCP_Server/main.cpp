#include"des.h"
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<thread>

#pragma comment(lib,"ws2_32.lib")

//发送数据的子线程
void sendData(SOCKET sClient) {
	string data;
	while (true) {
		getline(cin, data);
		cout << "Server>>>" << data << endl;
		data = encrypt(data);
		send(sClient, data.data(), data.length(), 0);
	}
}

//接收数据的父线程
int main() {
	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0) {
		cout << "Initialize WSA failed !" << endl;
		return 0;
	}

	//创建套接字
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET) {
		cout << "Create listen socket failed !" << endl;
		return 0;
	}

	//绑定IP和端口
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	inet_pton(AF_INET, INADDR_ANY, &sin.sin_addr.s_addr);
	sin.sin_port = htons(1234);
	if (::bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR) {
		cout << "Bind error !" << endl;
	}

	//监听
	if (listen(slisten, 5) == SOCKET_ERROR) {
		cout << "Listen error !" << endl;
		return 0;
	}


	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);

	L1:
	while (true) {
		cout << "Wait for connection..." << endl;
		sClient = accept(slisten, (SOCKADDR*)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET) {
			cout << "Accept error !" << endl;
			continue;
		}
		char addrLen[256];
		cout << "Connection accepted:" << inet_ntop(AF_INET, &remoteAddr.sin_addr, addrLen, sizeof(addrLen)) << endl;
		break;
	}

	//发送线程
	thread t(sendData, sClient);
	t.detach();

	while (true) {
		char revData[2048];
		int ret = recv(sClient, revData, 2048, 0);
		if (ret > 0) {
			revData[ret] = '\0';
			cout << "Client>>>" << decrypt(revData) << endl;
		}
		else {
			cout << "System: Client quit." << endl;
			goto L1;
		}
	}

	closesocket(sClient);
	closesocket(slisten);
	WSACleanup();

	return 0;
}