#include <iostream>
#include "layout.hpp"
#include "io.hpp"
int main(int argc, char const *argv[]){

    Layout layout;
    io::readLayout(&layout, argv[1]);

    
    return 0;
}
