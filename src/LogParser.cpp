#include "LogParser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip> // For std::get_time

std::vector<LogEntry> LogParser::parseLogFile(const std::string& filePath) {
    std::vector<LogEntry> logEntries;
    std::ifstream file(filePath);
    std::string line;

    // Skip header
    if (std::getline(file, line)) {
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string field;
            LogEntry entry;

            // Timestamp
            std::getline(ss, field, ',');
            std::tm tm = {};
            std::stringstream ssts(field);
            ssts >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
            entry.timestamp = std::chrono::system_clock::from_time_t(std::mktime(&tm));

            // Response ID
            std::getline(ss, entry.responseId, ',');

            // Latency
            std::getline(ss, field, ',');
            entry.latency = std::stod(field);

            // Tokens Used
            std::getline(ss, field, ',');
            entry.tokensUsed = std::stoi(field);

            // Response
            std::getline(ss, entry.response);
            // remove quotes
            if(entry.response.front() == '"' && entry.response.back() == '"') {
                entry.response = entry.response.substr(1, entry.response.size() - 2);
            }


            logEntries.push_back(entry);
        }
    } else {
        std::cerr << "Could not read log file or it is empty: " << filePath << std::endl;
    }

    return logEntries;
}
