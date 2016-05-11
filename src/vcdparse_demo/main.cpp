#include <iostream>
#include <map>

#include "vcdparse.hpp"

using namespace vcdparse;

void print_vcd(const VcdData& vcd_data);
void print_header(const Header& header);
void print_vars(const VcdData& vcd_data);
void print_changes(const VcdData& vcd_data);

int main(int argc, char** argv) {

    if(argc < 2 || (argc == 3 && std::string(argv[2]) != "-print") || argc > 3) {
        std::cout << "Usage: " << argv[0] << " vcd_file" << " [-print]\n";
        return 1;
    }

    vcdparse::Loader vcd_loader;
    bool loaded = vcd_loader.load(argv[1]);
    if(loaded) {
        std::cout << "Successfully loaded VCD\n";

        const auto& vcd_data = vcd_loader.get_vcd_data();

        if(argc == 3 && std::string(argv[2]) == "-print") {
            print_vcd(vcd_data);
        }
        return 0;
    } else {
        std::cout << "Failed to load VCD\n";
        return 1;
    }
}

void print_vcd(const VcdData& vcd_data) {
    print_header(vcd_data.header());
    print_vars(vcd_data);
    print_changes(vcd_data);
}

void print_header(const Header& header) {
    std::cout << "$date\n";
    std::cout << header.date() << "\n";
    std::cout << "$end\n";
    std::cout << "$version\n";
    std::cout << header.version() << "\n";
    std::cout << "$end\n";
    std::cout << "$timescale\n";
    std::cout << header.timescale() << "\n";
    std::cout << "$end\n";
}
void print_vars(const VcdData& vcd_data) {
    for(const auto& var : vcd_data.vars()) {
        std::cout << "$var ";
        std::cout << var.type() << " ";
        std::cout << var.width() << " ";
        std::cout << var.str_id() << " ";
        std::cout << var.name() << " ";
        std::cout << "$end\n";
    }
    std::cout << "$enddefinitions $end\n";
}

void print_changes(const VcdData& vcd_data) {
    std::map<Var::Id,std::string> var_id_to_str;
    for(const auto& var : vcd_data.vars()) {
        var_id_to_str[var.id()] = var.str_id();
    }

    size_t curr_time = 0;
    std::cout << "#" << curr_time << std::endl;
    for(const auto& time_val : vcd_data.time_values()) {
        if(time_val.time() != curr_time) {
            curr_time = time_val.time();
            std::cout << "#" << curr_time << "\n";
        }
        std::cout << time_val.value() << var_id_to_str[time_val.var_id()] << "\n";
    }
}
