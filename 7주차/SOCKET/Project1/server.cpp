#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib , "ws2_32.lib")
#include <stdio.h>
#include <WinSock2.h>
#include <process.h>
#include <vector>
using namespace std;	// vector는 std:: 안에 있습니다..

vector<SOCKET> clnt_list;//clnt list를 저장하기 위해서 vactor 를 사용.



void cdecl RecvThread(void* p)
{
	SOCKET sock = (SOCKET)p;
	char buf[256];
	while (1) {
		//-----------클라이언트로부터 수신------------
		int recvsize = recv(sock, buf, sizeof(buf), 0);
		if (recvsize <= 0)		break;
		//------------------------------------------------
		buf[recvsize] = '\0';
		printf("%s\n", buf);
		//---------- 각 클라이언트에게 전송------------------
		for (int i = 0; i < clnt_list.size(); i++) {
			/*if (clnt_list[i] != sock) {
				int sendsize = send(clnt_list[i], buf, strlen(buf), 0);
			}*/
			//내 전화번호하고 같으면 보내지 말라.
		}
		//-----------------------------------------------
	}
	printf("접속 종료\n");
	//------------vector에 있는 데이터 지우기-----------
	vector<SOCKET>::iterator iter = clnt_list.begin();
	for (int i = 0; i < clnt_list.size(); i++) {
		if (clnt_list[i] == sock) {
			clnt_list.erase(iter);
			break;
		}
		iter++;
	}
	//------------소켓 해제---------------------
	closesocket(sock);
	//----------------------------------------
}


int main()
{
	//-------소켓 라이브러리 불러오기(?)--------
	WSADATA wsaData;
	int retval = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (retval != 0) {
		printf("WSAStartup() Error\n");
		return 0;
	}
	//-------------------------------------------

	//----------소켓 생성--------------
	SOCKET serv_sock;
	serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serv_sock == SOCKET_ERROR) {
		printf("socket() Error\n");
		return 0;
	}
	//-----------------------------------

	//--------서버(자신)의 소켓 정보 입력------------
	SOCKADDR_IN serv_addr = { 0 }; // 초기화
	serv_addr.sin_family = AF_INET; // IP 사용
	serv_addr.sin_port = htons(13389); // 포트 4000번
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 주소는 알아서 찾기
	//------------------------------------------------

	//-----------인터넷에 연결---------------------
	retval = bind(serv_sock, (SOCKADDR*)&serv_addr, sizeof(SOCKADDR));//bind는 개통을 했으니까 연락이 올때까지 기달려라.
	if (retval == SOCKET_ERROR) {
		printf("bind() Error\n");
		return 0;
	}
	//-------------------------------------------
	//-----------대기인원 설정-----------------
	listen(serv_sock, 5);		// 5명까지만 대기할 수 있게 함...
	//-------------------------------------------
	SOCKADDR_IN clnt_addr = { 0 };
	int size = sizeof(SOCKADDR_IN);

	while (1)
	{
		//-------------클라이언트 접속 대기, connect를 하면 리턴함-------------
		SOCKET clnt_sock = accept(serv_sock, (SOCKADDR*)&clnt_addr, &size);
		if (clnt_sock == SOCKET_ERROR)
		{
			printf("accept() Error\n");
			continue;
		}
		//----------------------------------------------------------------------

		//----------------vector 에 데이터 넣기------------
		clnt_list.push_back(clnt_sock);
		//--------------------------------------------------

		printf("클라이언트 접속\n");
		printf("IP : %s, Port : %d\n", inet_ntoa(clnt_addr.sin_addr), clnt_addr.sin_port);
		//-----------수신 스레드 생성-------------
		_beginthread(RecvThread, NULL, (void*)clnt_sock);
		//-----------------------------------------
	}
	//----------소켓 닫음---------------
	closesocket(serv_sock);
	//-----------------------------------

	//-------라이브러리 해제(?)---------
	WSACleanup();
	//----------------------------------
}



