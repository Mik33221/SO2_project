#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
#include <algorithm>

std::vector<int> clients;
std::mutex clients_mutex;
std::vector<std::string> message_history;
std::mutex history_mutex;

void send_history(int client_socket) {
    std::lock_guard<std::mutex> lock(history_mutex);
    for (const auto& msg : message_history) {
        send(client_socket, msg.c_str(), msg.size(), 0);
    }
}

void broadcast(const std::string& message, int sender) {
    {
        std::lock_guard<std::mutex> lock(history_mutex);
        message_history.push_back(message);
    }
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (int client : clients) {
        if (client != sender) {
            send(client, message.c_str(), message.size(), 0);
        }
    }
}

void handle_client(int client_socket) {
    send_history(client_socket);
    char buffer[1024];
    int bytes_received;
    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        std::string msg(buffer);
        broadcast(msg, client_socket);
    }
    // Usuwanie klienta po rozłączeniu
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
    }
    close(client_socket);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Socket creation failed!" << std::endl;
        return 1;
    }

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(25565);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed!" << std::endl;
        close(server_socket);
        return 1;
    }

    if (listen(server_socket, SOMAXCONN) < 0) {
        std::cerr << "Listen failed!" << std::endl;
        close(server_socket);
        return 1;
    }

    std::cout << "Serwer chatu uruchomiony na porcie 25565..." << std::endl;

    while (true) {
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket < 0) {
            std::cerr << "Accept failed!" << std::endl;
            continue;
        }
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(client_socket);
        }
        std::thread(handle_client, client_socket).detach();
    }

    close(server_socket);
    return 0;
} 