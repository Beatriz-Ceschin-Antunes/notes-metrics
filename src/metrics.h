#ifndef METRICS_H
#define METRICS_H

#include <atomic>
#include <QString>

class Metrics
{
public:
    static Metrics& instance();

    void incNoteOpened();
    QString toPrometheusText() const;

private:
    Metrics() = default;
    std::atomic<quint64> m_noteOpenTotal{0};
};

#endif // METRICS_H
