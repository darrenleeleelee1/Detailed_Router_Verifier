#include <iostream>
#include "layout.hpp"
#include "io.hpp"
#include "verifier.hpp"
int main(int argc, char const *argv[]){

    Layout layout;
    io::readLayout(&layout, argv[1]);

    std::cout << "::: Checking segment overlappingn. :::\n";
    bool so = verifier::checkSegmentOverlapping(&layout);
    std::cout << "Segment overlapping: " << (so ? "Pass" : "Fail") << ".\n";

    std::cout << "::: Checking net openning and connectivity. :::\n";
    auto [no, c] = verifier::checkNetOpenAndConnectivity(&layout);
    std::cout << "Net openning: " << (no ? "Pass" : "Fail") << ".\n";
    std::cout << "Net connectivity: " << (c ? "Pass" : "Fail") << ".\n";


    std::cout << "---Summary---\n";
    if(!so) std::cout << "Segment overlapping!, please check error message.\n";
    if(!no) std::cout << "Net openning!, please check error message.\n";
    if(!c) std::cout << "Net not connected.\n";
    if(so && no && c) std::cout << "All Passed!\n";
    return 0;
}
