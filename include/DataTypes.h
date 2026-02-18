#ifndef DATATYPES_H
#define DATATYPES_H

#include <string>
#include <vector>
#include <chrono>

// Standardized Cognitive Metrics Interface (SCMI)
// Every monitored system must expose this.
struct SCMIEntry {
    std::chrono::system_clock::time_point timestamp;
    std::string module_origin;
    std::string output;
    double confidence;
    std::string reasoning_trace_hash;
    double inference_time;
    std::string internal_state_hash;
};

// Failure Mode Classification
enum class FailureMode {
    NONE,
    HALLUCINATION_ESCALATION,
    OVERCONFIDENCE_DRIFT,
    COGNITIVE_THRASHING,
    DECISION_STAGNATION,
    STRUCTURAL_INCOHERENCE,
    CASCADING_INFERENCE_COLLAPSE
};

// Cognitive Telemetry Stream Schema
struct CognitiveTelemetry {
    std::chrono::system_clock::time_point timestamp;
    std::string module_origin;
    double confidence_score;
    double entropy_level;
    int reasoning_depth;
    double inference_latency;
    double prediction_error;
    double contradiction_index;
    double stability_gradient;
    double resource_load;
    double risk_projection;

    // Integrity verification
    std::string hash_chain;
};

// Cognitive Observability Infrastructure (COI) Metrics
struct COIMetrics {
    double csi; // Cognitive Stability Index
    double dv;  // Drift Velocity
    double cce; // Confidence Calibration Error
    double scs; // Structural Coherence Score
    double prg; // Predictive Reliability Gradient
    double rie; // Risk of Irreversible Error
    double systemTrustScore;
    double instabilityAcceleration;
    FailureMode primaryFailureMode;
};

// Legacy support (to be phased out or mapped)
struct LogEntry {
    std::chrono::system_clock::time_point timestamp;
    std::string responseId;
    double latency;
    int tokensUsed;
    std::string response;

    // For mapping to SCMI
    double confidence = 1.0;
};

struct MonitoringMetrics {
    double averageLatency;
    int totalTokensUsed;
    double coherenceScore;
    COIMetrics coi; // Embedded COI metrics
};

#endif // DATATYPES_H
