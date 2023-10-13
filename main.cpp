#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Estructura de un punto en el tablero
struct Point {
    int x;
    int y;
};

// Función para enviar datos al servidor
bool sendToServer(const std::string& serverAddress, int serverPort, const std::string& data) {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(serverAddress.c_str());

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error: No se pudo conectar al servidor." << std::endl;
        return false;
    }

    send(clientSocket, data.c_str(), data.size(), 0);
    close(clientSocket);
    return true;
}

int main() {
    std::string serverAddress = "127.0.0.1";  // Dirección IP del servidor
    int serverPort = 12345;  // Puerto del servidor

    // aqui va la lógica para crear el tablero y seleccionar puntos de inicio y destino.

    // Por ejemplo:
    std::vector<Point> board;
    Point start = {0, 0};
    Point end = {9, 9};

    // aqui va la lógica para convertir el tablero y los puntos en una cadena de datos.

    // Por ejemplo:
    std::string data = "tablero_y_puntos_de_inicio_y_destino";

    if (sendToServer(serverAddress, serverPort, data)) {
        std::cout << "Datos enviados al servidor con éxito." << std::endl;
    } else {
        std::cerr << "Error al enviar datos al servidor." << std::endl;
    }

    return 0;
}