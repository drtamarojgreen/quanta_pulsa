#ifndef LOGPARSER_H
#define LOGPARSER_H

#include <string>
#include <vector>
#include "DataTypes.h"

class LogParser {
public:
    static std::vector<LogEntry> parseLogFile(const std::string& filePath);
};

#endif // LOGPARSER_H
