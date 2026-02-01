#include <QTcpServer>
#include <QTcpSocket>
#include "Metrics.h"
#include <MetricsServer.h>

MetricsServer::MetricsServer(quint16 port, QObject* parent): QObject(parent),m_server(new QTcpServer(this))
{
    connect(m_server, &QTcpServer::newConnection, this, [this]() {
        auto* sock = m_server->nextPendingConnection();
        connect(sock, &QTcpSocket::readyRead, sock, [sock]() {
            const QByteArray body = Metrics::instance().toPrometheusText().toUtf8();
            QByteArray resp =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/plain\r\n"
                    "Content-Length: " + QByteArray::number(body.size()) + "\r\n\r\n";
            resp += body;
            sock->write(resp);
            sock->disconnectFromHost();
        });
    });
    m_server->listen(QHostAddress::LocalHost, 9100);
};
