#ifndef METRICS_H
#define METRICS_H

#include <atomic>
#include <QString>
#include <QSettings>

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
    void incSessionsTotal();
    void incNotesEdits();
    void addSessionDurationSeconds(quint64 seconds);
    void loadSessionData(QSettings *db);
    void storeSessionData(QSettings *db);
    QString toPrometheusText() const;

private:
    bool enabled = true;
    std::vector<Metric> m_metrics;
    std::atomic<quint64> m_noteOpenTotal{0}; // number of distinct notes opened in a session
    std::atomic<quint64> m_noteCreated{0}; // number of notes created in a session
    std::atomic<quint64> m_sessionsTotal{0}; // total number of sessions
    std::atomic<quint64> m_sessionDurationSecondsTotal{0}; // duration of each session
    std::atomic<uint64_t> m_editsTotal{0}; // edits made to notes in a session
};

#endif // METRICS_H
