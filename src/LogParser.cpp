#include "LogParser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip> // For std::get_time

std::vector<LogEntry> LogParser::parseLogFile(const std::string& filePath) {
    std::vector<LogEntry> logEntries;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return logEntries;
    }

    std::string line;
    // Skip header
    if (std::getline(file, line)) {
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string field;
            LogEntry entry;

            try {
                // Timestamp
                if (!std::getline(ss, field, ',')) continue;
                std::tm tm = {};
                std::stringstream ssts(field);
                if (!(ssts >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ"))) continue;
                // Note: mktime uses local time. In a production COI,
                // a UTC-aware conversion like timegm should be used for certifiability.
                entry.timestamp = std::chrono::system_clock::from_time_t(std::mktime(&tm));

                // Response ID
                if (!std::getline(ss, entry.responseId, ',')) continue;

                // Latency
                if (!std::getline(ss, field, ',')) continue;
                entry.latency = std::stod(field);

                // Tokens Used
                if (!std::getline(ss, field, ',')) continue;
                entry.tokensUsed = std::stoi(field);

                // Response
                if (std::getline(ss, entry.response)) {
                    if(entry.response.size() >= 2 && entry.response.front() == '"' && entry.response.back() == '"') {
                        entry.response = entry.response.substr(1, entry.response.size() - 2);
                    }
                }

                logEntries.push_back(entry);
            } catch (...) {
                // Ignore malformed lines for robustness in mission-critical ingestion
                continue;
            }
        }
    }

    return logEntries;
}

std::vector<SCMIEntry> LogParser::parseSCMILogFile(const std::string& filePath) {
    std::vector<SCMIEntry> entries;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Could not open SCMI log file: " << filePath << std::endl;
        return entries;
    }

    std::string line;
    // Expected header: timestamp,module_origin,output,confidence,reasoning_trace_hash,inference_time,internal_state_hash
    if (std::getline(file, line)) {
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string field;
            SCMIEntry entry;

            try {
                // Timestamp
                if (!std::getline(ss, field, ',')) continue;
                std::tm tm = {};
                std::stringstream ssts(field);
                if (!(ssts >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ"))) continue;
                entry.timestamp = std::chrono::system_clock::from_time_t(std::mktime(&tm));

                // Module Origin
                if (!std::getline(ss, entry.module_origin, ',')) continue;

                // Output
                if (!std::getline(ss, entry.output, ',')) continue;
                if(entry.output.size() >= 2 && entry.output.front() == '"' && entry.output.back() == '"') {
                    entry.output = entry.output.substr(1, entry.output.size() - 2);
                }

                // Confidence
                if (!std::getline(ss, field, ',')) continue;
                entry.confidence = field.empty() ? 0.0 : std::stod(field);

                // Reasoning Trace Hash
                if (!std::getline(ss, entry.reasoning_trace_hash, ',')) continue;

                // Inference Time
                if (!std::getline(ss, field, ',')) continue;
                entry.inference_time = field.empty() ? 0.0 : std::stod(field);

                // Internal State Hash
                if (!std::getline(ss, entry.internal_state_hash, ',')) continue;

                entries.push_back(entry);
            } catch (...) {
                continue;
            }
        }
    }
    return entries;
}
