#ifndef METRICS_H
#define METRICS_H

#include <atomic>
#include <QString>

enum class MetricType {
    Counter,
    Gauge
};

struct Metric {
    QString name;
    QString help;
    MetricType type;
    std::atomic<quint64>* value;
};

class Metrics
{
public:
    static Metrics& instance();
    Metrics();

    void setEnabled(bool m_enabled);
    bool isEnabled() const;

    void incNoteOpened();
    void incNoteCreated();
    QString toPrometheusText() const;

private:
    bool enabled = true;
    std::vector<Metric> m_metrics;
    std::atomic<quint64> m_noteOpenTotal{0}; // number of distinct notes opened
    std::atomic<quint64> m_noteCreated{0}; // number of notes created in a session
};

#endif // METRICS_H
