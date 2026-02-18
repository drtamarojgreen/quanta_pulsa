#include "gtest/gtest.h"
#include "LegacyMonitoring.h"
#include <vector>

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
