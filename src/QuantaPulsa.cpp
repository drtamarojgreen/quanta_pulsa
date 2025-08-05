#include <iostream>
#include <vector>
#include <string>
#include <numeric> // For std::accumulate
#include "DataTypes.h"
#include "LogParser.h"

// --- Configuration for Alerting ---
struct AlertThresholds {
    double maxAverageLatency;
    double minCoherenceScore;
};

// Function to calculate monitoring metrics from log entries
MonitoringMetrics calculateMetrics(const std::vector<LogEntry>& entries) {
    MonitoringMetrics metrics = {0.0, 0, 0.0};

    if (entries.empty()) {
        return metrics;
    }

    // Calculate total latency and tokens
    double totalLatency = 0.0;
    for (const auto& entry : entries) {
        totalLatency += entry.latency;
        metrics.totalTokensUsed += entry.tokensUsed;
    }

    // Calculate average latency
    metrics.averageLatency = totalLatency / entries.size();

    // Set a placeholder for coherence score
    metrics.coherenceScore = 0.95; // Placeholder value

    return metrics;
}

// Function to check for alerts
void checkAlerts(const MonitoringMetrics& metrics, const AlertThresholds& thresholds) {
    std::cout << "\n--- Checking for Alerts ---" << std::endl;
    bool noAlerts = true;

    if (metrics.averageLatency > thresholds.maxAverageLatency) {
        std::cout << "ALERT: Average latency (" << metrics.averageLatency
                  << "s) exceeds threshold of " << thresholds.maxAverageLatency << "s." << std::endl;
        noAlerts = false;
    }

    if (metrics.coherenceScore < thresholds.minCoherenceScore) {
        std::cout << "ALERT: Coherence score (" << metrics.coherenceScore
                  << ") is below threshold of " << thresholds.minCoherenceScore << "." << std::endl;
        noAlerts = false;
    }

    if (noAlerts) {
        std::cout << "No alerts." << std::endl;
    }
}

int main() {
    const std::string logFilePath = "logs.csv";
    auto logEntries = LogParser::parseLogFile(logFilePath);

    if (logEntries.empty()) {
        std::cerr << "No log entries found. Exiting." << std::endl;
        return 1;
    }

    MonitoringMetrics metrics = calculateMetrics(logEntries);

    // --- Dashboard Output ---
    std::cout << "--- Monitoring Metrics ---" << std::endl;
    std::cout << "Average Latency: " << metrics.averageLatency << "s" << std::endl;
    std::cout << "Total Tokens Used: " << metrics.totalTokensUsed << std::endl;
    std::cout << "Coherence Score: " << metrics.coherenceScore << std::endl;

    // --- Alerting ---
    AlertThresholds thresholds = {1.0, 0.9}; // Configure thresholds here
    checkAlerts(metrics, thresholds);

    return 0;
}
