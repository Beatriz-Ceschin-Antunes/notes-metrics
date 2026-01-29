#include <QTcpServer>
#include <QTcpSocket>
#include "Metrics.h"

class MetricsServer : public QObject {
public:
    MetricsServer(QObject* parent = nullptr) : QObject(parent) {
        connect(&server, &QTcpServer::newConnection, this, [this]() {
            auto* sock = server.nextPendingConnection();
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
        server.listen(QHostAddress::LocalHost, 9100);
    }

private:
    QTcpServer server;
};
