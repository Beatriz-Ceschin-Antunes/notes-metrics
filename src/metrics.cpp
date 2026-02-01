#include "metrics.h"

Metrics& Metrics::instance() {
    static Metrics inst;
    return inst;
}

Metrics::Metrics() {
    m_metrics.push_back({
            "note_open_total",
            "Total number of note open events",
            MetricType::Counter,
            &m_noteOpenTotal
    });

    m_metrics.push_back({
            "notes_created_total",
            "Total number of notes created",
            MetricType::Counter,
            &m_noteCreated
    });

    m_metrics.push_back({
            "session_duration_seconds_total",
            "Total session duration in seconds",
            MetricType::Counter,
            &m_sessionDurationSecondsTotal
    });
}

/*!
 * \brief Metrics::incNoteOpened()
 * Increments count as new note is opened.
 */
void Metrics::incNoteOpened() {
    if (!enabled)
        return;

    m_noteOpenTotal++;
}

/*!
 * \brief Metrics::incNoteCreated()
 * Increments count as new note is created.
 */
void Metrics::incNoteCreated() {
    if (!enabled)
        return;

    m_noteCreated++;
}

/*!
 * \brief Metrics::incSessionsTotal()
 * Increments count as new session is launched.
 */
void Metrics::incSessionsTotal() {
    if (!enabled)
        return;

    m_sessionsTotal++;
}

/*!
 * \brief Metrics::addSessionDurationSeconds(quint64 seconds)
 * Increments duration total as session duration is recorded.
 */
void Metrics::addSessionDurationSeconds(quint64 seconds) {
    if (!enabled)
        return;

    m_sessionDurationSecondsTotal += seconds;
}

/*!
 * \brief Metrics::loadSessionData(QSettings db)
 * Load data for session duration.
 */
void Metrics::loadSessionData(QSettings *db) {
    m_sessionDurationSecondsTotal.store(db->value(QStringLiteral("metrics/sessionDurationSecondsTotal"),0).toULongLong(),std::memory_order_relaxed);
}

/*!
 * \brief Metrics::storeSessionData(QSettings db)
 * Stores data for session duration.
 */
void Metrics::storeSessionData(QSettings *db) {
    db->setValue(QStringLiteral("metrics/sessionDurationSecondsTotal"),QVariant::fromValue<qulonglong>(m_sessionDurationSecondsTotal.load(std::memory_order_relaxed)));

    db->sync();
}

/*!
 * \brief Metrics::toPrometheusText()
 * Exposes metrics to Prometheus.
 */
QString Metrics::toPrometheusText() const {
    QString out;

    for (const auto& metric : m_metrics) {
        out += "# HELP " + metric.name + " " + metric.help + "\n";

        switch (metric.type) {
        case MetricType::Counter:
            out += "# TYPE " + metric.name + " counter\n";
            break;
        case MetricType::Gauge:
            out += "# TYPE " + metric.name + " gauge\n";
            break;
        }

        out += metric.name + " "
                + QString::number(metric.value->load(std::memory_order_relaxed))
                + "\n";
    }

    return out;
}

/*!
 * \brief Metrics::setEnabled(bool enabled)
 * Sets value for enabled flag.
 */
void Metrics::setEnabled(bool m_enabled){
    enabled = m_enabled;
}

/*!
 * \brief Metrics::isEnabled()
 * Retrives value for enabled flag.
 */
bool Metrics::isEnabled() const{
    return enabled;
}
