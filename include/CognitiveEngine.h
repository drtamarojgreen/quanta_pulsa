#ifndef COGNITIVEENGINE_H
#define COGNITIVEENGINE_H

#include "DataTypes.h"
#include <vector>

class CognitiveEngine {
public:
    static COIMetrics computeMetrics(const std::vector<SCMIEntry>& entries);
    static FailureMode classifyFailure(const COIMetrics& metrics, const SCMIEntry& lastEntry);
    static double calculateSystemTrustScore(const COIMetrics& metrics);
};

#endif // COGNITIVEENGINE_H
