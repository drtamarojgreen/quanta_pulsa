#include "CognitiveEngine.h"
#include <cmath>
#include <algorithm>
#include <numeric>

COIMetrics CognitiveEngine::computeMetrics(const std::vector<SCMIEntry>& entries) {
    COIMetrics metrics = {};
    if (entries.empty()) return metrics;

    const auto& last = entries.back();

    // 1. CSI - Cognitive Stability Index
    // Based on confidence and latency stability
    double avgConfidence = 0;
    double latVar = 0;
    double avgLat = 0;
    for (const auto& e : entries) {
        avgConfidence += e.confidence;
        avgLat += e.inference_time;
    }
    avgConfidence /= entries.size();
    avgLat /= entries.size();

    for (const auto& e : entries) {
        latVar += std::pow(e.inference_time - avgLat, 2);
    }
    latVar /= entries.size();

    metrics.csi = avgConfidence * (1.0 / (1.0 + latVar));

    // 2. DV - Drift Velocity
    if (entries.size() > 1) {
        const auto& prev = entries[entries.size() - 2];
        double dt = std::chrono::duration<double>(last.timestamp - prev.timestamp).count();
        if (dt > 0) {
            metrics.dv = (last.confidence - prev.confidence) / dt;
        }
    }

    // 3. SCS - Structural Coherence Score
    // Mocked as a function of confidence and reasoning trace stability
    metrics.scs = last.confidence * 0.98; // Placeholder

    // 4. CCE - Confidence Calibration Error
    // Heuristic: high confidence with high latency variance might indicate overconfidence
    metrics.cce = std::abs(last.confidence - metrics.csi);

    // 5. Instability Acceleration
    if (entries.size() > 2) {
        const auto& prev = entries[entries.size() - 2];
        const auto& prev2 = entries[entries.size() - 3];
        double dt1 = std::chrono::duration<double>(last.timestamp - prev.timestamp).count();
        double dt2 = std::chrono::duration<double>(prev.timestamp - prev2.timestamp).count();
        if (dt1 > 0 && dt2 > 0) {
            double dv1 = (last.confidence - prev.confidence) / dt1;
            double dv2 = (prev.confidence - prev2.confidence) / dt2;
            metrics.instabilityAcceleration = (dv1 - dv2) / ((dt1 + dt2) / 2.0);
        }
    }

    // 6. PRG - Predictive Reliability Gradient
    metrics.prg = metrics.dv * 0.7 + metrics.instabilityAcceleration * 0.3;

    // 7. RIE - Risk of Irreversible Error
    metrics.rie = std::exp(std::abs(metrics.instabilityAcceleration)) / (1.0 + metrics.csi + 1e-6);
    metrics.rie = std::min(1.0, metrics.rie / 10.0); // Bounded

    metrics.systemTrustScore = calculateSystemTrustScore(metrics);
    metrics.primaryFailureMode = classifyFailure(metrics, last);

    return metrics;
}

FailureMode CognitiveEngine::classifyFailure(const COIMetrics& metrics, const SCMIEntry& lastEntry) {
    if (metrics.rie > 0.8) return FailureMode::CASCADING_INFERENCE_COLLAPSE;
    if (metrics.instabilityAcceleration < -0.5) return FailureMode::COGNITIVE_THRASHING;
    if (metrics.dv < -0.3) return FailureMode::HALLUCINATION_ESCALATION;
    if (metrics.cce > 0.4 && lastEntry.confidence > 0.9) return FailureMode::OVERCONFIDENCE_DRIFT;
    if (metrics.scs < 0.5) return FailureMode::STRUCTURAL_INCOHERENCE;

    return FailureMode::NONE;
}

double CognitiveEngine::calculateSystemTrustScore(const COIMetrics& metrics) {
    // Weighted aggregation
    double score = 0.3 * metrics.csi +
                   0.2 * metrics.scs +
                   0.2 * (1.0 - std::abs(metrics.cce)) +
                   0.1 * (1.0 - std::abs(metrics.dv)) +
                   0.2 * (1.0 - metrics.rie);
    return std::clamp(score, 0.0, 1.0);
}
