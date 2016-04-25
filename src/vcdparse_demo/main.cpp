#include <iostream>

#include "vcdparse.hpp"

int main(int argc, char** argv) {

    if(argc != 2) {
        std::cout << "Usage: " << argv[0] << " vcd_file" << "\n";
        return 1;
    }

    vcdparse::Loader vcd_loader;
    bool loaded = vcd_loader.load(argv[1]);
    if(loaded) {
        std::cout << "Successfully loaded VCD\n";

        auto vcd_data = vcd_loader.get_vcd_data();
        return 0;
    } else {
        std::cout << "Failed to load VCD\n";
        return 1;
    }
}
