// Task1_Client.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"Ws2_32.lib") 

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define BUFF_SIZE 2048
#define USERNAME_SIZE 15
#define PASSWORD_SIZE 256

//define client message types
enum clientMsgType {
	CLOSE = 0,
	LOGIN,
	LOGOUT
};

//define server message types
enum serverMsgType {
	LOGIN_SUCCESS = 10,
	INVALID_ID,
	INCORRECT_PASSWORD_2,
	INCORRECT_PASSWORD_1,
	ACCOUNT_LOCKED,
	LOGOUT_SUCCESS,
};

//define transmission message struct
typedef struct transmissionMessage {
	int messageType;
	char content[BUFF_SIZE];
} transmissionMessage;

//handle message received from server
int serverMsgHandle(int msgType) {

	/*
	//cast received string buffer to message struct
	transmissionMessage* serverMsg;
	serverMsg = (transmissionMessage*)buff;

	msgType = serverMsg->messageType;
	*/
	//message handle
	switch (msgType)
	{
	case LOGIN_SUCCESS:
		printf("Successfully logged in!\n");
		return LOGIN_SUCCESS;
		break;
	case INVALID_ID:
		printf("Username does not exist / not registered!\n\n");
		return INVALID_ID;
		break;
	case INCORRECT_PASSWORD_2:
		//get number of attempts left (user gets 3 password attempts until the account is locked)
		printf("Password incorrect!\n");
		printf("2 attempts left on this account\n\n");
		return INCORRECT_PASSWORD_2;
		break;
	case INCORRECT_PASSWORD_1:
		//get number of attempts left (user gets 3 password attempts until the account is locked)
		printf("Password incorrect!\n");
		printf("1 attempts left on this account\n\n");
		return INCORRECT_PASSWORD_1;
		break;
	case ACCOUNT_LOCKED:
		printf("The account has been locked!\n\n");
		return ACCOUNT_LOCKED;
		break;
	case LOGOUT_SUCCESS:
		printf("Successfully logged out!\n");
		return LOGOUT_SUCCESS;
		break;
	}
}


int main(int argc, char* argv[])
{
	//Command check (3 arguments)
	if (argc != 3) {
		printf("UNKNOWN COMMAND\n");
		return 1;
	}


	//Initiate Winsock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData))
		printf("Version is not supported\n");

	//Construct socket
	SOCKET client;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//Set time-out for receiving to tv (mili-second)
	int tv = 50000;
	setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char*)(&tv), sizeof(int));


	//Specify server address
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[2]));
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

	//Request to connect to server
	if (connect(client, (sockaddr*)&serverAddr, sizeof(serverAddr))) {
		printf("ERROR!: Cannot connect to server\n");
		WSACleanup();
		return 1;
	}
	printf("Connected to server!\n");

	//Communicate with server
	char buff[sizeof(transmissionMessage)];
	char username_str[USERNAME_SIZE];
	char password_str[PASSWORD_SIZE];
	int ret, msgType = -1, cmd;



	//CLIENT APPLICATION
	printf("CLIENT APPLICATION\n");

	while (1) {//Login and logout loop until get Exit command
		printf("Command: \n");
		printf("1. Login\n");
		printf("2. Exit\n");

		fflush(stdin);
		cmd = _getch();

		while ((cmd != '1') && (cmd != '2')) {
			printf("INVALID COMMAND!\n");
			fflush(stdin);
			cmd = _getch();
		};

		//break loop if get Exit command
		if (cmd == '2') {
			
			break;
		}

		//Login
		else {
			printf("Login to Server:\n");

			strcpy_s(buff, sizeof(transmissionMessage), "");
			//Enter and send login informations until success
			while (msgType != LOGIN_SUCCESS) {
				printf("Username: ");//get username
				gets_s(username_str, USERNAME_SIZE);
				//username containing space character is not allowed
				while (strchr(username_str, ' ') != NULL) {
					printf("\n\nUsername cannot contain space character. Re-enter Username: ");
					gets_s(username_str, USERNAME_SIZE);
				}

				printf("\nPassword: ");//get password
				gets_s(password_str, PASSWORD_SIZE);

				//cast transmission buffer to defined message struct
				transmissionMessage* loginMessage;
				loginMessage = (transmissionMessage*)buff;

				//message content
				loginMessage->messageType = LOGIN;
				strcpy_s(loginMessage->content, username_str);
				loginMessage->content[strlen(username_str)] = ' ';//space character to separate username string and password string
				loginMessage->content[strlen(username_str) + 1] = '\0';
				strcat_s(loginMessage->content, password_str);

				ret = send(client, buff, sizeof(transmissionMessage), 0);
				if (ret == SOCKET_ERROR) {
					printf("ERROR!: Cannot send message. Code %d\n", WSAGetLastError());
					closesocket(client);
					WSACleanup();
					return 1;
				}


				ret = recv(client, buff, sizeof(transmissionMessage), 0);
				if (ret == SOCKET_ERROR) {
					if (WSAGetLastError() == WSAETIMEDOUT) {
						printf("Time-out!\n");
					}
					else printf("ERROR!: Cannot receive message\n");
				}


				//get respond message type (code)
				transmissionMessage* serverMsg;
				serverMsg = (transmissionMessage*)buff;
				msgType = serverMsg->messageType;

				//handle message type
				serverMsgHandle(msgType);
			}
		}


		//Logged in. Get command
		printf("Command: \n");
		printf("1. Logout\n");
		printf("2. Exit\n");
		fflush(stdin);
		cmd = _getch();

		while ((cmd != '1') && (cmd != '2')) {
			printf("INVALID COMMAND!\n");
			fflush(stdin);
			cmd = _getch();
		};

		//break loop if get Exit command
		if (cmd == '2') {
			
			break;
		}
		else {
			//send logout message until success
			while (msgType != LOGOUT_SUCCESS) {
				transmissionMessage* logoutMessage;
				logoutMessage = (transmissionMessage*)buff;
				logoutMessage->messageType = LOGOUT;

				ret = send(client, buff, sizeof(transmissionMessage), 0);
				if (ret == SOCKET_ERROR) {
					printf("ERROR!: Cannot send message. Code %d\n", WSAGetLastError());
					closesocket(client);
					WSACleanup();
					return 1;
				}

				ret = recv(client, buff, sizeof(transmissionMessage), 0);
				if (ret == SOCKET_ERROR) {
					if (WSAGetLastError() == WSAETIMEDOUT) {
						printf("Time-out!\n");
					}
					else printf("ERROR!: Cannot receive message\n");
				}

				transmissionMessage* serverMsg;
				serverMsg = (transmissionMessage*)buff;

				msgType = serverMsg->messageType;

				serverMsgHandle(msgType);
			}
		}
	}

	printf("Socket Closed!\n");

	//Shutdown Socket
	shutdown(client, SD_BOTH);

	//Close Socket
	closesocket(client);

	//Terminate Winsock
	WSACleanup();

	return 0;
}