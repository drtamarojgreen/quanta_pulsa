#include "TelemetryExporter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <functional>

void TelemetryExporter::exportTelemetry(const CognitiveTelemetry& t, const std::string& filePath) {
    std::ofstream file(filePath, std::ios::app);
    if (!file.is_open()) return;

    // CSV format for telemetry stream
    auto tp = std::chrono::system_clock::to_time_t(t.timestamp);
    file << std::put_time(std::gmtime(&tp), "%Y-%m-%dT%H:%M:%SZ") << ","
         << t.module_origin << ","
         << t.confidence_score << ","
         << t.entropy_level << ","
         << t.reasoning_depth << ","
         << t.inference_latency << ","
         << t.prediction_error << ","
         << t.contradiction_index << ","
         << t.stability_gradient << ","
         << t.resource_load << ","
         << t.risk_projection << ","
         << t.hash_chain << "\n";
}

CognitiveTelemetry TelemetryExporter::generateTelemetry(const SCMIEntry& entry, const COIMetrics& metrics, const std::string& prevHash) {
    CognitiveTelemetry t;
    t.timestamp = entry.timestamp;
    t.module_origin = entry.module_origin;
    t.confidence_score = entry.confidence;
    t.entropy_level = 1.0 - metrics.scs;
    t.reasoning_depth = 5; // Placeholder for actual depth analysis
    t.inference_latency = entry.inference_time;
    t.prediction_error = metrics.cce;
    t.contradiction_index = 1.0 - metrics.scs;
    t.stability_gradient = metrics.dv;
    t.resource_load = 0.5; // Placeholder for actual resource monitoring
    t.risk_projection = metrics.rie;

    // --- INTEGRITY VERIFICATION ---
    // NOTE: For mission-critical COI in regulated environments (Energy, Defense),
    // a cryptographic hash (e.g., SHA-256) is mandatory for certifiability.
    // The following implementation uses std::hash as a structural placeholder
    // for the hash-chaining logic to ensure tamper-resistance.

    std::stringstream ss;
    ss << prevHash << entry.internal_state_hash << metrics.csi;
    size_t hashValue = std::hash<std::string>{}(ss.str());

    std::stringstream hashSs;
    hashSs << std::hex << hashValue;
    t.hash_chain = hashSs.str();

    return t;
}
