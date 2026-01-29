#ifndef METRICSSERVER_H
#define METRICSSERVER_H

#include <QObject>

class QTcpServer;

class MetricsServer final : public QObject {

public:
    explicit MetricsServer(quint16 port = 9100, QObject* parent = nullptr);

private:
    QTcpServer* m_server = nullptr;
};


#endif // METRICSSERVER_H
