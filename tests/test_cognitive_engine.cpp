#include "gtest/gtest.h"
#include "CognitiveEngine.h"
#include <vector>
#include <chrono>

TEST(CognitiveEngineTest, TestStabilityMetrics) {
    std::vector<SCMIEntry> entries;
    auto now = std::chrono::system_clock::now();

    entries.push_back({now, "test", "out", 0.99, "h1", 0.1, "s1"});
    entries.push_back({now + std::chrono::seconds(5), "test", "out", 0.98, "h2", 0.11, "s2"});
    entries.push_back({now + std::chrono::seconds(10), "test", "out", 0.97, "h3", 0.12, "s3"});

    COIMetrics metrics = CognitiveEngine::computeMetrics(entries);

    EXPECT_GT(metrics.csi, 0.5); // Should be relatively high
    EXPECT_GT(metrics.systemTrustScore, 0.7);
    EXPECT_EQ(metrics.primaryFailureMode, FailureMode::NONE);
}

TEST(CognitiveEngineTest, TestDegradationDetection) {
    std::vector<SCMIEntry> entries;
    auto now = std::chrono::system_clock::now();

    // Rapidly dropping confidence and increasing latency
    entries.push_back({now, "test", "out", 0.9, "h1", 0.1, "s1"});
    entries.push_back({now + std::chrono::seconds(5), "test", "out", 0.6, "h2", 1.0, "s2"});
    entries.push_back({now + std::chrono::seconds(10), "test", "out", 0.2, "h3", 5.0, "s3"});

    COIMetrics metrics = CognitiveEngine::computeMetrics(entries);

    EXPECT_LT(metrics.dv, 0.0);
    EXPECT_GT(metrics.rie, 0.0);
}
