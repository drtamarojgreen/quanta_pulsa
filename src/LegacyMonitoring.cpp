#include "LegacyMonitoring.h"

MonitoringMetrics calculateMetrics(const std::vector<LogEntry>& entries) {
    MonitoringMetrics metrics = {0.0, 0, 0.0, {}};

    if (entries.empty()) {
        return metrics;
    }

    double totalLatency = 0.0;
    for (const auto& entry : entries) {
        totalLatency += entry.latency;
        metrics.totalTokensUsed += entry.tokensUsed;
    }
    metrics.averageLatency = totalLatency / entries.size();
    metrics.coherenceScore = 0.95;

    return metrics;
}
