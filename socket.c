#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Iphlpapi.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#define _DEFAULT_PORT 8888
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <Windows.h>
#include <iphlpapi.h>
#include <netioapi.h>
#include <icmpapi.h>
#include <stdio.h>

SOCKET startHTTPlistener(int port) 
{
	struct sockaddr_in server_addr;
	server_addr.sin_port = htons(port);

	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	server_addr.sin_family = AF_INET;

	struct	WSAData wsa;
	int i = WSAStartup(2, &wsa);
	
	SOCKET _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == SOCKET_ERROR)
	{
		fprintf(stderr,"[Error] %d method socket()",GetLastError());
	}
	
	int res = bind(_socket, &server_addr, sizeof(SOCKADDR));
	if (res == -1)
	{
		fprintf(stderr,"[Error] %d method bind()", GetLastError());
	}

	res = listen(_socket, 100);
	if (res == -1)
	{
		fprintf(stderr,"[Error] %d method listen()", GetLastError());
	}
	return _socket;
}
int sendHttpGET(int socket,const char *html) 
{
	char *header ="HTTP/1.x 200 OK \n Transfer-Encoding: chunked \n\n <html><body>";
	char* ending = "</html></body>";
	char *response = malloc(strlen(html)+strlen(ending)+strlen(header));

	strcpy(response, header);
	strcat(response,html);
	strcat(response,ending);


	return send(socket, response, strlen(response), NULL);
}

int main(void) 
{
	char host[10];
	SOCKET socket = startHTTPlistener(_DEFAULT_PORT);
	
        SOCKET s =SOCKET_ERROR;
        printf("listening...\n");
	
	s = accept(socket, NULL, NULL);
	//signal
	printf("connected\n");
	Beep(750, 750);
	
	sendHttpGET(s,"<h1>Total Page Views:10</h1>");

	printf("socket :%d\n", s);

	return 0;
}
