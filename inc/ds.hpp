#pragma once
#include <vector>
#include <map>
#include "layout.hpp"
class Grid{
public:
    int net_id = -1;
    std::vector<Segment_Draw*> segments;
    Grid() {
        segments.resize(0);
    }
    ~Grid() {}
};
