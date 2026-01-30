#ifndef METRICS_H
#define METRICS_H

#include <atomic>
#include <QString>

class Metrics
{
public:
    static Metrics& instance();

    void setEnabled(bool m_enabled);
    bool isEnabled() const;

    void incNoteOpened();
    QString toPrometheusText() const;

private:
    Metrics() = default;
    bool enabled = true;
    std::atomic<quint64> m_noteOpenTotal{0};
};

#endif // METRICS_H
