#include "Common.h"
#include <list>

//initiates a session for the client
//sesion[OUT]: the session after initiates
void InitiateSession(struct SESSION* session) {
	session->connSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	session->connSock = 0;
}

//Check for the existence of an ID
//listSession[IN]: the list of session of client that connected to server 
//ID[IN]         : the ID needs to check
bool CheckRamdomID(map<string, SESSION*> listSession, char* ID) {
	for (pair<string, SESSION*> session : listSession)
		if (!strcmp(ID, session.second->ID)) return false;
	return true;
}

//create a ramdom ID
char* CreateRamdomID() {
	char* result = new char[20];
	srand((int)time(0));
	for (int i = 0; i < 20; i++) {
		int element = rand() % 26 + 97;
		result[i] = (char)element;
	}
	result[20] = 0;
	return result;
}

//convert string to chars
//input[IN]: 
char* StringToChars(string input) {
	int length = input.length();
	char* result = new char[length];
	for (int i = 0; i < length; i++) result[i] = input[i];
	result[length] = 0;
	return result;
}
