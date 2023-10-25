#include <QApplication>
#include <iostream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <QtWidgets>
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>

QString Path = "/home/andresc/Escritorio/Client-Samurai/";


// Estructura de un punto en el tablero
struct Point {
    int x;
    int y;
};


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

    // Crear una matriz para representar el tablero
    const int boardSize = 10;
    std::vector<std::vector<int>> gameBoard(boardSize, std::vector<int>(boardSize, 0));

    // Crear un layout de cuadrícula (10x10)
    QGridLayout *gridLayout = new QGridLayout(&window);

    // Crear un QLabel con la imagen del samurái redimensionada a 50x50
    QPixmap samuraiPixmap(Path + "rojo1.png");
    samuraiPixmap = samuraiPixmap.scaled(50, 50, Qt::KeepAspectRatio);
    QLabel *samuraiLabel = new QLabel(&window);
    samuraiLabel->setPixmap(samuraiPixmap);


    auto sendCoordinatesToServer = [&serverAddress, serverPort](int row, int col) {
        std::string coordinates = std::to_string(row) + "," + std::to_string(col);
        if (sendToServer(serverAddress, serverPort, coordinates)) {
            std::cout << "Coordenadas enviadas al servidor: " << coordinates << std::endl;
        } else {
            std::cerr << "Error al enviar coordenadas al servidor." << std::endl;
        }
    };


    // Función para actualizar la posición del samurái en la ventana
    auto updateSamuraiPosition = [&samuraiLabel](int newRow, int newCol) {
        // Actualiza la posición del samurái en la ventana
        samuraiLabel->setGeometry(newCol * 50, newRow * 50, 50, 50);
    };

    // Agregar QLabel del samurái al layout en la posición inicial (0, 0)
    gridLayout->addWidget(samuraiLabel, 0, 0);
    gameBoard[0][0] = 1;

    // Agregar etiquetas como fondo para el tablero (10x10)
    for (int row = 0; row < boardSize; ++row) {
        for (int col = 0; col < boardSize; ++col) {
            QLabel *cellLabel = new QLabel;
            cellLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
            cellLabel->setFixedSize(50, 50);
            gridLayout->addWidget(cellLabel, row, col);

        }
    }

    // Mostrar la ventana
    window.show();

    return app.exec();
}
