#ifndef LEGACYMONITORING_H
#define LEGACYMONITORING_H

#include "DataTypes.h"
#include <vector>

MonitoringMetrics calculateMetrics(const std::vector<LogEntry>& entries);

#endif // LEGACYMONITORING_H
