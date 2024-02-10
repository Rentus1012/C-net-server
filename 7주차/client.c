#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment (lib, "ws2_32.lib")//라이브러리를 포함.

#include <stdio.h>
#include <WinSock2.h>//윈도우 소켓 통신 함수
//랜카드를 제어하는 헤더
#include <process.h>
//program: 하드에 저장된 컴파일러를 돌리는것
//실행중인 프로그램을 프로세스.
//채팅 프로그램은 메시지를 송, 수신을 동시에 처리함.

void cdecl RecvThread (void * p)//실행중일때 메시지를 쌓아놓고, 끝날때는 싹다 지우셈.(메시지 수신 함수)
{
	SOCKET sock = (SOCKET)p;//인터넷을 연결하는 선: 소켓
	char buf[256];
	while(1)
	{
		//FILE *fp = fopen("log.txt", "at+");//이거는 텍스트 로그를 txt로 저장.
		//-----------서버로부터 수신------------
		int recvsize = recv(sock,buf,sizeof(buf),0);//메시지가 도착하면 버퍼에 담아달라.
        //resvize 는 날라온 문자의 크기를 저장.
		if(recvsize <= 0)
		{
			printf("접속종료\n");//에러나면 꺼라.
			break;
		}
		//------------------------------------------------
		buf[recvsize] = '\0';//winsock에는 실제 데이터를 보내야함. 
		printf("\r%s\n>>",buf);
		//fprintf(fp, "%s\n", buf);
		//fclose(fp);
	}
}

int main()
{
	//-------소켓 라이브러리 불러오기(?)--------
	WSADATA wsaData;
	int retval = WSAStartup(MAKEWORD(2,2),&wsaData);
	if(retval != 0)
	{
		printf("WSAStartup() Error\n");
		return 0;
	}
	//------------------------------------------

	//---------소켓생성-------- 
	SOCKET serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	//TCP를 이용한 소켓
	//-------------------------

	//---------서버 정보 입력--------------------
	SOCKADDR_IN serv_addr;
	serv_addr.sin_family = AF_INET;	 // IP주소를 이용하고
	serv_addr.sin_port = htons(13389); // 소켓은 4000번에(실제로 이걸 사용하면 안됨.)
	serv_addr.sin_addr.s_addr = inet_addr("203.230.193.158"); //로컬 서버의 ip 주소는 127.0.0.1(그 ip로 통신하게 하라.)
	//--------------------------------------------

	//---------서버 연결------------
	retval = connect(serv_sock,(SOCKADDR*)&serv_addr,sizeof(SOCKADDR));
	if(retval == SOCKET_ERROR)
	{
		printf("connect() Error\n");
		return 0;
	}

    char nick[20];
	printf("닉네임 >> ");
	gets(nick);

	_beginthread(RecvThread,0,(void*)serv_sock);//계속 백그라운드로 돌려라.



    char buf[256] = {0};
	char str[256];//사실은 여기로빼야함.

	while(1)
	{

		//char buf[256] = {0};

		//char str[256];
		printf(">> ");
		gets(str);
		sprintf(buf,"[%s] %s",nick,str);//메모리로 출력을 해라.buf로.

		//---------서버에 메시지 전송---------------
		int sendsize = send(serv_sock,buf,strlen(buf),0);
		if(sendsize <= 0)
			break;
		//------------------------------------------

	}

	//----------소켓 닫음------------------
	closesocket(serv_sock);
	//-------------------------------------

	//---------라이브러리 해제(?)---------
	WSACleanup();
	//-------------------------------------
}


