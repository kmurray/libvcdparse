#include <iostream>

#include "vcdparse.hpp"

using namespace vcdparse;

void print_vcd(const VcdData& vcd_data);

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
        print_vcd(vcd_data);
        return 0;
    } else {
        std::cout << "Failed to load VCD\n";
        return 1;
    }
}

void print_vcd(const VcdData& vcd_data) {
    for(auto& signal_values : vcd_data.signal_values()) {
        auto& var = signal_values.var();
        std::cout << "Var: " << var.name() << " (" << var.id() << ")\n";

        auto& tvs = signal_values.time_values();
        size_t len = 1000;
        len = std::min(len, tvs.size());
        for(size_t i = 0; i < len; ++i) {
            std::cout << "\tTime: " << tvs[i].time() << " Values: " << tvs[i].value() << "\n";
        }
    }
}
