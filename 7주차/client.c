#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment (lib, "ws2_32.lib")//���̺귯���� ����.

#include <stdio.h>
#include <WinSock2.h>//������ ���� ��� �Լ�
//��ī�带 �����ϴ� ���
#include <process.h>
//program: �ϵ忡 ����� �����Ϸ��� �����°�
//�������� ���α׷��� ���μ���.
//ä�� ���α׷��� �޽����� ��, ������ ���ÿ� ó����.

void cdecl RecvThread (void * p)//�������϶� �޽����� �׾Ƴ���, �������� �ϴ� �����.(�޽��� ���� �Լ�)
{
	SOCKET sock = (SOCKET)p;//���ͳ��� �����ϴ� ��: ����
	char buf[256];
	while(1)
	{
		//FILE *fp = fopen("log.txt", "at+");//�̰Ŵ� �ؽ�Ʈ �α׸� txt�� ����.
		//-----------�����κ��� ����------------
		int recvsize = recv(sock,buf,sizeof(buf),0);//�޽����� �����ϸ� ���ۿ� ��ƴ޶�.
        //resvize �� ����� ������ ũ�⸦ ����.
		if(recvsize <= 0)
		{
			printf("��������\n");//�������� ����.
			break;
		}
		//------------------------------------------------
		buf[recvsize] = '\0';//winsock���� ���� �����͸� ��������. 
		printf("\r%s\n>>",buf);
		//fprintf(fp, "%s\n", buf);
		//fclose(fp);
	}
}

int main()
{
	//-------���� ���̺귯�� �ҷ�����(?)--------
	WSADATA wsaData;
	int retval = WSAStartup(MAKEWORD(2,2),&wsaData);
	if(retval != 0)
	{
		printf("WSAStartup() Error\n");
		return 0;
	}
	//------------------------------------------

	//---------���ϻ���-------- 
	SOCKET serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	//TCP�� �̿��� ����
	//-------------------------

	//---------���� ���� �Է�--------------------
	SOCKADDR_IN serv_addr;
	serv_addr.sin_family = AF_INET;	 // IP�ּҸ� �̿��ϰ�
	serv_addr.sin_port = htons(13389); // ������ 4000����(������ �̰� ����ϸ� �ȵ�.)
	serv_addr.sin_addr.s_addr = inet_addr("203.230.193.158"); //���� ������ ip �ּҴ� 127.0.0.1(�� ip�� ����ϰ� �϶�.)
	//--------------------------------------------

	//---------���� ����------------
	retval = connect(serv_sock,(SOCKADDR*)&serv_addr,sizeof(SOCKADDR));
	if(retval == SOCKET_ERROR)
	{
		printf("connect() Error\n");
		return 0;
	}

    char nick[20];
	printf("�г��� >> ");
	gets(nick);

	_beginthread(RecvThread,0,(void*)serv_sock);//��� ��׶���� ������.



    char buf[256] = {0};
	char str[256];//����� ����λ�����.

	while(1)
	{

		//char buf[256] = {0};

		//char str[256];
		printf(">> ");
		gets(str);
		sprintf(buf,"[%s] %s",nick,str);//�޸𸮷� ����� �ض�.buf��.

		//---------������ �޽��� ����---------------
		int sendsize = send(serv_sock,buf,strlen(buf),0);
		if(sendsize <= 0)
			break;
		//------------------------------------------

	}

	//----------���� ����------------------
	closesocket(serv_sock);
	//-------------------------------------

	//---------���̺귯�� ����(?)---------
	WSACleanup();
	//-------------------------------------
}


