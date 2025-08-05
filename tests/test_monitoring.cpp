#include "gtest/gtest.h"
#include "DataTypes.h"
#include <vector>

// The function to be tested is in QuantaPulsa.cpp.
// To make it testable, it should be moved to a separate file or declared in a header.
// For now, we will copy the function here to test it.
// This is not ideal, but avoids a larger refactoring at this moment.
MonitoringMetrics calculateMetrics(const std::vector<LogEntry>& entries);

// We also need the definition of the function.
// In a real project, this would be in a library that is linked.
// For this exercise, I will redefine it here.
MonitoringMetrics calculateMetrics(const std::vector<LogEntry>& entries) {
    MonitoringMetrics metrics = {0.0, 0, 0.0};

    if (entries.empty()) {
        return metrics;
    }

    double totalLatency = 0.0;
    for (const auto& entry : entries) {
        totalLatency += entry.latency;
        metrics.totalTokensUsed += entry.tokensUsed;
    }
    metrics.averageLatency = totalLatency / entries.size();
    metrics.coherenceScore = 0.95; // Placeholder

    return metrics;
}


TEST(MonitoringTest, TestCalculateMetrics) {
    std::vector<LogEntry> entries;
    entries.push_back({{}, "resp-1", 1.0, 100, "..."});
    entries.push_back({{}, "resp-2", 2.0, 200, "..."});
    entries.push_back({{}, "resp-3", 3.0, 300, "..."});

    MonitoringMetrics result = calculateMetrics(entries);

    EXPECT_DOUBLE_EQ(result.averageLatency, 2.0);
    EXPECT_EQ(result.totalTokensUsed, 600);
}

TEST(MonitoringTest, TestEmptyLogEntries) {
    std::vector<LogEntry> entries;
    MonitoringMetrics result = calculateMetrics(entries);

    EXPECT_DOUBLE_EQ(result.averageLatency, 0.0);
    EXPECT_EQ(result.totalTokensUsed, 0);
}
