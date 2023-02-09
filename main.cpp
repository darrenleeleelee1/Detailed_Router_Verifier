#include <iostream>
#include "layout.hpp"
#include "io.hpp"
#include "verifier.hpp"
int main(int argc, char const *argv[]){

    Layout layout;
    io::readLayout(&layout, argv[1]);

    std::cout << "Start checking segment overlapping.\n";
    bool so = verifier::checkSegmentOverlapping(&layout);
    std::cout << "Segment overlapping: " << (so ? "Pass" : "Fail") << ".\n";

    std::cout << "Start checking net openning and connectivity.\n";
    auto [no, c] = verifier::checkNetOpenAndConnectivity(&layout);
    std::cout << "Net openning and connectivity: " << (no ? "Pass" : "Fail") << ".\n";


    std::cout << "---Summary---\n";
    if(!so) std::cout << "Segment overlapping!, please check error message.\n";
    if(!no) std::cout << "Net openning!, please check error message.\n";
    if(!c) std::cout << "Net not connected.\n";
    if(so && no && c) std::cout << "Passed!\n";
    return 0;
}
