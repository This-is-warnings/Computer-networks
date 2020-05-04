
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <locale>
#include <sstream> 
#include <iomanip>
#include <fstream>
using namespace std;
#pragma comment(lib,"ws2_32.lib")

SOCKET s;
struct sockaddr_in addr;
hostent* d_addr;
char text[1024];
void prt()								// функций для приёма сообщений и вывода на экран
{
	int n = 0;
	char buffer[1000];
	int i = 0;
	bool m = false;
	while (true) {
		n = recv(s, buffer, 1000, 0);   // считываем ответ "пачками" по 1000 байт

		while (i != n)
		{
			cout << buffer[i];						// выводим посимвольно			
			i++;
		}
		i = 0;
		if (n < 1000) break;				// заканчиваем приём, если в этот раз приняли меньше 1000 байт
	}
}
int main() {
	setlocale(LC_ALL, "");
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws)!=0)
	{
		printf("Error in WSAStartup(...)\n");
		system("pause");
		return 1;
	}
	

	// создаем сокет 
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET)
	{
		printf("Error in socket(...)\n");
		system("pause");
		return 1;
	}

	// получаем адрес сервера 
	d_addr = gethostbyname("mail.ngs.ru");
	if (d_addr == NULL)
	{
		printf("Error in gethostbyname(...)\n");
		system("pause");
		return 1;
	};

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = *((unsigned long*)d_addr->h_addr);
	addr.sin_port = htons(110); // подключаемся по 110 порту

	// устанавливаем соединение 
	if (SOCKET_ERROR == (connect(s, (sockaddr*)& addr,
		sizeof(addr))))
	{
		printf("Error in connect(...)\n");
		system("pause");
		return 1;
	}

	 
	char user[1024];     // строка для поля user
	strcpy(user, "user ");
	char pass[1024];     // строка для поля pass
	strcpy(pass, "pass ");

	recv(s, text, sizeof(text), 0); // сообщение о подключении к серверу
	printf(text);
	cout << "Enter name:  ";
	cin >> text;		// вводим имя
	strcat(user, text);
	char text2[1024];
	strcat(user, "\r\n");
	send(s, user, strlen(user), 0);	//отправляем строка user *name*
	prt();	// принимаем сообщение

	cout << "Enter pass:  "; // вводим пароль
	cin >> text;
	strcat(pass, text);
	strcat(pass, "\r\n");
	send(s, pass, strlen(pass), 0); // отправляем пароль
	
	prt(); // принимаем сообщение


	char list[10];
	strcpy(list, "list\r\n");	// строка для LIST
	int choise;
	char ch[5];
	while (true) {
		cout << endl << "--------------------------------------------------------------\n";
		cout << "1 - Просмотреть список сообщений\n2 - Прочитать сообщение\n";
		cin >> choise;
		cout << "--------------------------------------------------------------\n";
		if (choise == 1) {
			send(s, list, strlen(list), 0); // отправить запрос на просмотр сообщений
			prt(); // принимаем ответ
		}
		else if (choise == 2)
		{
			cout << endl << "--------------------------------------------------------------\n";
			cout << " Введите номер сообщения, которое хотите прочитать\n";
			cin >> ch;
			cout << "--------------------------------------------------------------\n";
			char retr[10];
			strcpy(retr, "retr "); // строка для RETR
			strcat(retr, ch);
			strcat(retr, "\r\n");
			send(s, retr, strlen(retr), 0); // отправляем запрос на просмотр сообщения
			prt(); // просмотреть запрос требуется два раза =)
			prt();
		
		}
	}
	
	closesocket(s);
	system("pause");
	return 0;
}