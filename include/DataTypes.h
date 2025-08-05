#ifndef DATATYPES_H
#define DATATYPES_H

#include <string>
#include <vector>
#include <chrono>

// Represents a single entry from the LLM agent's communication log.
struct LogEntry {
    std::chrono::system_clock::time_point timestamp;
    std::string responseId;
    double latency;       // in seconds
    int tokensUsed;
    std::string response; // The actual response text for future analysis
};

// Holds the calculated monitoring metrics.
struct MonitoringMetrics {
    double averageLatency;
    int totalTokensUsed;
    double coherenceScore; // Placeholder for now
};

#endif // DATATYPES_H
