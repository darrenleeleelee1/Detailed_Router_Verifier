#pragma once
#include "layout.hpp"
namespace verifier{
    bool checkSegmentOverlapping(Layout *layout);
    std::pair<bool, bool> checkNetOpenAndConnectivity(Layout *layout);
} // namespace verifier