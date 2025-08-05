#include "gtest/gtest.h"
#include "DataTypes.h"
#include <iostream>
#include <sstream>
#include <string>

// The function to be tested is in QuantaPulsa.cpp.
// To make it testable, it should be moved to a separate file or declared in a header.
// For now, we will copy the function here to test it.
struct AlertThresholds {
    double maxAverageLatency;
    double minCoherenceScore;
};

void checkAlerts(const MonitoringMetrics& metrics, const AlertThresholds& thresholds);

// Redefinition of the function for testing purposes.
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

// Test fixture for capturing cout
class AlertingTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Redirect cout to our stringstream
        sbuf = std::cout.rdbuf();
        std::cout.rdbuf(buffer.rdbuf());
    }

    void TearDown() override {
        // Restore cout
        std::cout.rdbuf(sbuf);
    }

    std::stringstream buffer;
    std::streambuf* sbuf;
};

TEST_F(AlertingTest, HighLatencyAlert) {
    MonitoringMetrics metrics = {1.5, 500, 0.95};
    AlertThresholds thresholds = {1.0, 0.9};
    checkAlerts(metrics, thresholds);
    std::string output = buffer.str();
    EXPECT_NE(output.find("ALERT: Average latency"), std::string::npos);
}

TEST_F(AlertingTest, LowCoherenceAlert) {
    MonitoringMetrics metrics = {0.8, 500, 0.85};
    AlertThresholds thresholds = {1.0, 0.9};
    checkAlerts(metrics, thresholds);
    std::string output = buffer.str();
    EXPECT_NE(output.find("ALERT: Coherence score"), std::string::npos);
}

TEST_F(AlertingTest, NoAlerts) {
    MonitoringMetrics metrics = {0.8, 500, 0.95};
    AlertThresholds thresholds = {1.0, 0.9};
    checkAlerts(metrics, thresholds);
    std::string output = buffer.str();
    EXPECT_NE(output.find("No alerts."), std::string::npos);
}
