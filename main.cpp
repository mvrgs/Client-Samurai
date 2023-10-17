#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <QtWidgets>

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

int main(int argc, char *argv[]) {
    std::string serverAddress = "127.0.0.1";  // Dirección IP del servidor
    int serverPort = 12345;  // Puerto del servidor

    QApplication app(argc, argv);

    // Crear la ventana principal
    QWidget window;
    window.setWindowTitle("Samurai Stronghold GBP");

    // Crear un layout de cuadrícula (10x10)
    QGridLayout *gridLayout = new QGridLayout(&window);

    std::vector<Point> board;
    Point start = {0, 0};
    Point end = {9, 9};

    // Función para enviar las coordenadas al servidor
    auto sendCoordinatesToServer = [&serverAddress, serverPort](int row, int col) {
        std::string coordinates = std::to_string(row) + "," + std::to_string(col);
        if (sendToServer(serverAddress, serverPort, coordinates)) {
            std::cout << "Coordenadas enviadas al servidor: " << coordinates << std::endl;
        } else {
            std::cerr << "Error al enviar coordenadas al servidor." << std::endl;
        }
    };

    // Crear el tablero y agregar botones
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            QPushButton *button = new QPushButton(QString("%1,%2").arg(row).arg(col));
            button->setFixedSize(70, 70);
            gridLayout->addWidget(button, row, col);

            // Conectar el evento clic del botón a la función de envío de coordenadas
            QObject::connect(button, &QPushButton::clicked, [row, col, &sendCoordinatesToServer]() {
                sendCoordinatesToServer(row, col);
            });

            // Agregar lógica para marcar el punto de inicio y destino
            if (row == start.y && col == start.x) {
                button->setText("Inicio");
            } else if (row == end.y && col == end.x) {
                button->setText("Destino");
            }
        }
    }

    window.setLayout(gridLayout);
    window.show();

    return app.exec();
}
