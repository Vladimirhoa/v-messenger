#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main(){
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // создали сокет

    sockaddr_in serverAddress; // создали адрес сокета
    serverAddress.sin_family = AF_INET; // сказали что передача c протоколом IPv4
    serverAddress.sin_port = htons(7001); // определили порт адреса
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // принимаем запросы с любого IP

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
        std::cout << "Failed to connect the socket to the server" << std::endl;
        return 1;
    } //привязали сокет к адресу
    else{
        std::cout << "The socket is open!" << std::endl;
    }

    listen(serverSocket, 3); // слушаем запросы, если в очереди больше 3 то отклоняем
    while (true){
        int clientSocket;
        if ((clientSocket = accept(serverSocket, nullptr, nullptr)) < 0){
            std::cout << "Couldn\'t connect to the client" << std::endl;
            return 1;
        }
        else{
            std::cout << "Client connect!" << std::endl;
            char buf[1024]; // Сообщение которое пришлёт клиент
            while(true){
                memset(buf, 0, 1024);
                int bytes_received = recv(clientSocket, buf, 1024, 0); // сколько байтов получили от клиента
                if (bytes_received <= 0) {
                    std::cout << "Client disconnected or error." << std::endl;
                    break; // Выходим из внутреннего цикла к accept
                }
                std::cout << "Received: " << bytes_received << " bytes" << std::endl;
                std::cout << "Client message:" << buf << std::endl;
                //выход при вводе exit
                if (strncmp(buf, "exit", 4) == 0) {
                    std::cout << "Client requested exit." << std::endl;
                    break;
                }

            }
            close(clientSocket);
            std::cout << "Connection closed, waiting for next client..." << std::endl;
        }
    }




}