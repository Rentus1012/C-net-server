#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib , "ws2_32.lib")
#include <stdio.h>
#include <WinSock2.h>
#include <process.h>
#include <vector>
using namespace std;	// vector�� std:: �ȿ� �ֽ��ϴ�..

vector<SOCKET> clnt_list;//clnt list�� �����ϱ� ���ؼ� vactor �� ���.



void cdecl RecvThread(void* p)
{
	SOCKET sock = (SOCKET)p;
	char buf[256];
	while (1) {
		//-----------Ŭ���̾�Ʈ�κ��� ����------------
		int recvsize = recv(sock, buf, sizeof(buf), 0);
		if (recvsize <= 0)		break;
		//------------------------------------------------
		buf[recvsize] = '\0';
		printf("%s\n", buf);
		//---------- �� Ŭ���̾�Ʈ���� ����------------------
		for (int i = 0; i < clnt_list.size(); i++) {
			/*if (clnt_list[i] != sock) {
				int sendsize = send(clnt_list[i], buf, strlen(buf), 0);
			}*/
			//�� ��ȭ��ȣ�ϰ� ������ ������ ����.
		}
		//-----------------------------------------------
	}
	printf("���� ����\n");
	//------------vector�� �ִ� ������ �����-----------
	vector<SOCKET>::iterator iter = clnt_list.begin();
	for (int i = 0; i < clnt_list.size(); i++) {
		if (clnt_list[i] == sock) {
			clnt_list.erase(iter);
			break;
		}
		iter++;
	}
	//------------���� ����---------------------
	closesocket(sock);
	//----------------------------------------
}


int main()
{
	//-------���� ���̺귯�� �ҷ�����(?)--------
	WSADATA wsaData;
	int retval = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (retval != 0) {
		printf("WSAStartup() Error\n");
		return 0;
	}
	//-------------------------------------------

	//----------���� ����--------------
	SOCKET serv_sock;
	serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock == SOCKET_ERROR) {
		printf("socket() Error\n");
		return 0;
	}
	//-----------------------------------

	//--------����(�ڽ�)�� ���� ���� �Է�------------
	SOCKADDR_IN serv_addr = { 0 }; // �ʱ�ȭ
	serv_addr.sin_family = AF_INET; // IP ���
	serv_addr.sin_port = htons(13389); // ��Ʈ 4000��
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // �ּҴ� �˾Ƽ� ã��
	//------------------------------------------------

	//-----------���ͳݿ� ����---------------------
	retval = bind(serv_sock, (SOCKADDR*)&serv_addr, sizeof(SOCKADDR));//bind�� ������ �����ϱ� ������ �ö����� ��޷���.
	if (retval == SOCKET_ERROR) {
		printf("bind() Error\n");
		return 0;
	}
	//-------------------------------------------
	//-----------����ο� ����-----------------
	listen(serv_sock, 5);		// 5������� ����� �� �ְ� ��...
	//-------------------------------------------
	SOCKADDR_IN clnt_addr = { 0 };
	int size = sizeof(SOCKADDR_IN);

	while (1)
	{
		//-------------Ŭ���̾�Ʈ ���� ���, connect�� �ϸ� ������-------------
		SOCKET clnt_sock = accept(serv_sock, (SOCKADDR*)&clnt_addr, &size);
		if (clnt_sock == SOCKET_ERROR)
		{
			printf("accept() Error\n");
			continue;
		}
		//----------------------------------------------------------------------

		//----------------vector �� ������ �ֱ�------------
		clnt_list.push_back(clnt_sock);
		//--------------------------------------------------

		printf("Ŭ���̾�Ʈ ����\n");
		printf("IP : %s, Port : %d\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);
		//-----------���� ������ ����-------------
		_beginthread(RecvThread, NULL, (void*)clnt_sock);
		//-----------------------------------------
	}
	//----------���� ����---------------
	closesocket(serv_sock);
	//-----------------------------------

	//-------���̺귯�� ����(?)---------
	WSACleanup();
	//----------------------------------
}



