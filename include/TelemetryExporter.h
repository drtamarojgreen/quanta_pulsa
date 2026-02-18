#ifndef TELEMETRYEXPORTER_H
#define TELEMETRYEXPORTER_H

#include "DataTypes.h"
#include <string>
#include <vector>

class TelemetryExporter {
public:
    static void exportTelemetry(const CognitiveTelemetry& telemetry, const std::string& filePath);
    static CognitiveTelemetry generateTelemetry(const SCMIEntry& entry, const COIMetrics& metrics, const std::string& prevHash);
};

#endif // TELEMETRYEXPORTER_H
