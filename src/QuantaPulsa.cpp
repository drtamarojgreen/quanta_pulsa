#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <iomanip>
#include <fstream>
#include "DataTypes.h"
#include "LogParser.h"
#include "CognitiveEngine.h"
#include "TelemetryExporter.h"

void printMetrics(const COIMetrics& metrics) {
    std::cout << "\n--- COI Metrics Update ---" << std::endl;
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "CSI (Stability):          " << metrics.csi << std::endl;
    std::cout << "DV (Drift Velocity):      " << metrics.dv << std::endl;
    std::cout << "CCE (Calibration Error):  " << metrics.cce << std::endl;
    std::cout << "SCS (Structural Coherence):" << metrics.scs << std::endl;
    std::cout << "PRG (Reliability Gradient):" << metrics.prg << std::endl;
    std::cout << "RIE (Risk of Irreversible Error): " << metrics.rie << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "SYSTEM TRUST SCORE:       " << metrics.systemTrustScore << std::endl;

    if (metrics.primaryFailureMode != FailureMode::NONE) {
        std::cout << "!!! ALERT: FAILURE MODE DETECTED: ";
        switch (metrics.primaryFailureMode) {
            case FailureMode::HALLUCINATION_ESCALATION: std::cout << "Hallucination Escalation"; break;
            case FailureMode::OVERCONFIDENCE_DRIFT: std::cout << "Overconfidence Drift"; break;
            case FailureMode::COGNITIVE_THRASHING: std::cout << "Cognitive Thrashing"; break;
            case FailureMode::DECISION_STAGNATION: std::cout << "Decision Stagnation"; break;
            case FailureMode::STRUCTURAL_INCOHERENCE: std::cout << "Structural Incoherence"; break;
            case FailureMode::CASCADING_INFERENCE_COLLAPSE: std::cout << "Cascading Inference Collapse"; break;
            default: std::cout << "Unknown"; break;
        }
        std::cout << " !!!" << std::endl;
    }
}

int main() {
    const std::string logFilePath = "scmi_logs.csv";
    const std::string telemetryPath = "telemetry_stream.csv";

    std::cout << "====================================================" << std::endl;
    std::cout << "  QuantaPulsa: Cognitive Observability Infrastructure" << std::endl;
    std::cout << "====================================================" << std::endl;

    auto scmiEntries = LogParser::parseSCMILogFile(logFilePath);

    if (scmiEntries.empty()) {
        std::cerr << "[ERROR] No SCMI log entries found in " << logFilePath << std::endl;
        std::cerr << "Ensure system exposes: {timestamp,origin,output,confidence,trace_hash,latency,state_hash}" << std::endl;
        return 1;
    }

    std::vector<SCMIEntry> history;
    std::string lastHash = "genesis_block_integrity_root";

    // Clear previous telemetry log for fresh run
    std::ofstream clearFile(telemetryPath, std::ios::trunc);
    clearFile.close();

    for (const auto& entry : scmiEntries) {
        history.push_back(entry);

        // Compute Cognitive Observability Metrics
        COIMetrics metrics = CognitiveEngine::computeMetrics(history);

        // Generate and export tamper-resistant telemetry
        CognitiveTelemetry telemetry = TelemetryExporter::generateTelemetry(entry, metrics, lastHash);
        TelemetryExporter::exportTelemetry(telemetry, telemetryPath);
        lastHash = telemetry.hash_chain;

        // Output to Enterprise Dashboard
        printMetrics(metrics);
    }

    std::cout << "\n[INFO] Audit-ready telemetry stream persisted to: " << telemetryPath << std::endl;
    std::cout << "Status: Operational. All mission-critical metrics computed." << std::endl;

    return 0;
}
