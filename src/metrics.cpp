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
