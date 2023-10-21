#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTextEdit>

class DaemonClient : public QMainWindow {
    Q_OBJECT

public:
    DaemonClient() {
        initUI();
        createConnection();
    }

    ~DaemonClient() {
        clientSocket->disconnectFromHost(); // Отключаемся от сервера перед удалением
        delete clientSocket; // Удаляем объект clientSocket
    }
private slots:
    void sendCommand() {
        QString command = inputText->text();
        clientSocket->write(command.toUtf8());
        inputText->clear();
    }

    void receiveMessage() {
        QString message = clientSocket->readAll();
        // if (message == "quit") {
        //     clientSocket->close();
        //     QCoreApplication::quit();
        // }
        // else 
        // {
            messageDisplay->append(message);
        // }
    }

private:
    DaemonClient(const DaemonClient&) = delete;  // Запрещаем конструктор копирования
    DaemonClient& operator=(const DaemonClient&) = delete;  // Запрещаем оператор присвоения

    void initUI() {
        setGeometry(100, 100, 400, 300); // Увеличьте высоту окна.
        setWindowTitle("Daemon Client");

        inputText = new QLineEdit(this);
        inputText->setGeometry(20, 20, 260, 30);

        sendButton = new QPushButton("Send", this);
        sendButton->setGeometry(290, 20, 80, 30);
        connect(sendButton, &QPushButton::clicked, this, &DaemonClient::sendCommand);

        // Добавьте QTextEdit для отображения сообщений.
        messageDisplay = new QTextEdit(this);
        messageDisplay->setGeometry(20, 70, 350, 180);
        messageDisplay->setReadOnly(true); // Чтобы пользователь не мог редактировать текст.
    }

    void createConnection() {
        serverAddress = QHostAddress("127.0.0.1");
        serverPort = 4242;

        clientSocket = new QTcpSocket(this);
        clientSocket->connectToHost(serverAddress, serverPort);

        connect(clientSocket, &QTcpSocket::readyRead, this, &DaemonClient::receiveMessage);
        connect(clientSocket, &QTcpSocket::disconnected, this, &DaemonClient::handleDisconnected);
    }

    void handleDisconnected() {
        messageDisplay->append("Server disconnected.");
        qDebug() << "Server disconnected.";
        clientSocket->disconnectFromHost();
        sendButton->setEnabled(false);
        // Выполните необходимые действия при отключении сервера.
    }

    QLineEdit*      inputText;
    QPushButton*    sendButton;
    QTextEdit*      messageDisplay;
    QTcpSocket*     clientSocket;
    QHostAddress    serverAddress;
    quint16         serverPort;
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    DaemonClient client;
    client.show();
    return app.exec();
}

#include "src_client.moc"
