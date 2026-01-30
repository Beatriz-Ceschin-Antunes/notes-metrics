#include "metrics.h"

Metrics& Metrics::instance() {
    static Metrics inst;
    return inst;
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
 * \brief Metrics::toPrometheusText()
 * Exposes metrics to Prometheus.
 */
QString Metrics::toPrometheusText() const {
    const quint64 count = m_noteOpenTotal.load(std::memory_order_relaxed);

    QString out;
    out += "# HELP note_open_total Total number of note open events\n";
    out += "# TYPE note_open_total counter\n";
    out += "note_open_total " + QString::number(count) + "\n";
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
