#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTextEdit>

#define LOCALHOST   "127.0.0.1"
#define PORT        4242

class DaemonClient : public QMainWindow
{
    Q_OBJECT

public:
    DaemonClient()
    {
        initUI();
        createConnection();
    }

    ~DaemonClient()
    {
        clientSocket->disconnectFromHost();
        delete clientSocket;
    }

private slots:
    void sendCommand()
    {
        QString command = inputText->text();
        clientSocket->write(command.toUtf8());
        inputText->clear();
    }

    void receiveMessage()
    {
        QString message = clientSocket->readAll();
        messageDisplay->append(message);
    }

private:
    DaemonClient(const DaemonClient&) = delete;
    DaemonClient& operator=(const DaemonClient&) = delete;

    void initUI()
    {
        setGeometry(100, 100, 400, 300);
        setWindowTitle("Daemon Client");

        inputText = new QLineEdit(this);
        inputText->setGeometry(20, 20, 260, 30);

        sendButton = new QPushButton("Send", this);
        sendButton->setGeometry(290, 20, 80, 30);
        connect(sendButton, &QPushButton::clicked, this, &DaemonClient::sendCommand);

        messageDisplay = new QTextEdit(this);
        messageDisplay->setGeometry(20, 70, 350, 180);
        messageDisplay->setReadOnly(true);
    }

    void createConnection()
    {
        serverAddress = QHostAddress(LOCALHOST);
        serverPort = PORT;

        clientSocket = new QTcpSocket(this);
        clientSocket->connectToHost(serverAddress, serverPort);

        connect(clientSocket, &QTcpSocket::readyRead, this, &DaemonClient::receiveMessage);
        connect(clientSocket, &QTcpSocket::disconnected, this, &DaemonClient::handleDisconnected);
    }

    void handleDisconnected()
    {
        messageDisplay->append("Server disconnected.");
        qDebug() << "Server disconnected.";
        clientSocket->disconnectFromHost();
        sendButton->setEnabled(false);
    }

    QLineEdit*      inputText;
    QPushButton*    sendButton;
    QTextEdit*      messageDisplay;
    QTcpSocket*     clientSocket;
    QHostAddress    serverAddress;
    quint16         serverPort;
};

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    DaemonClient client;
    client.show();
    return app.exec();
}

#include "src_client.moc"
