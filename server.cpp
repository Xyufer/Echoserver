#include "server.h"
#include <QDebug>

Server::Server(QObject *parent) : QObject(parent)
{
    m_leds = new Gpio(this);
    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::newConnection, this, &Server::myNewConnection);
}

void Server::start()
{
    m_server->listen(QHostAddress::Any, PORT);
    qDebug() << "Server lauscht auf port" << PORT;
}

void Server::myNewConnection()
{
    m_socket = m_server->nextPendingConnection();
    connect(m_socket, &QTcpSocket::readyRead, this, &Server::myServerRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &Server::myClientDisconnect);
    qDebug() << "Neue Client-Verbindung";
    m_leds->set(1);
}

void Server::myClientDisconnect()
{
    m_socket->close();
    qDebug() << "Client hat Verbindung getrennt";
    m_leds->set(0);
}

void Server::myServerRead()
{
    qDebug() << "Lese Client-Daten und sende sie zurueck";
    QByteArray message = m_socket->readAll();
    qDebug() << "Nachricht von Client: " << message;
    std::reverse(message.begin(), message.end());
    qDebug() << "Antwort an Client: " << message;
    m_socket->write(message);
    m_socket->write("\n\n");
}
