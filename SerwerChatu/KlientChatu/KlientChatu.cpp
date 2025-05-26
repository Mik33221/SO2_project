#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <string>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

// On recieving message print it to console
void receive_messages(SOCKET sock) {
    char buffer[1024];
    int bytes_received;
    while ((bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        std::cout << buffer << std::flush;
    }
    std::cout << "\nRozłączono z serwerem." << std::endl;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed!" << std::endl;
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        return 1;
    }

    // IP address from user
    std::string server_ip;
    std::cout << "Podaj IP serwera: 158.180.21.212";
    //std::cin >> server_ip;
    server_ip = "158.180.21.212";
    server_ip = "127.0.0.1";
    std::cin.ignore();

    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(25565);
    server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());

    // Try connecting to server
    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Nie można połączyć z serwerem!" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "Połączono z serwerem. Możesz pisać wiadomości." << std::endl;

    // Thread recieving messages
    std::thread receiver(receive_messages, sock);

    // Thread reading from user and sending
    std::string msg;
    while (std::getline(std::cin, msg)) {
        msg += '\n';
        send(sock, msg.c_str(), msg.size(), 0);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
} 