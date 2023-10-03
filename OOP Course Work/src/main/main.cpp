#include <iostream>
#include <string>
#include "../ethernet/TcpServer.hpp"

// ������� IPv4-������ � ����� � std::string
std::string getHostStr(const TcpServer::Client& client) {
    uint32_t ip = client.getHost();
    return std::string() + std::to_string(int(reinterpret_cast<char*>(&ip)[0])) + '.' +
        std::to_string(int(reinterpret_cast<char*>(&ip)[1])) + '.' +
        std::to_string(int(reinterpret_cast<char*>(&ip)[2])) + '.' +
        std::to_string(int(reinterpret_cast<char*>(&ip)[3])) + ':' +
        std::to_string(client.getPort());
}

int main() {
    // �������� ���������� �������
    TcpServer server(8080, [](DataBuffer data, TcpServer::Client& client) {
        std::cout << "(" << getHostStr(client) << ")[ " << data.size << " bytes ]: " << (char*)data.data_ptr << '\n';
        client.sendData("Hello, client!", sizeof("Hello, client!"));
        }, { 1, 1, 1 }); // Keep alive{��������:1s, ��������: 1s, ���-�� �������: 1};

    // ������ �������
    if (server.start() == TcpServer::status::up) {
        std::cout << "Server is up!" << std::endl;
        server.joinLoop(); //Joing to the client handling loop
    }
    else {
        std::cout << "Server start error! Error code:" << int(server.getStatus()) << std::endl;
        return -1;
    }

}