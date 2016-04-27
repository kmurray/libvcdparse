#include <iostream>
#include <map>

#include "vcdparse.hpp"

using namespace vcdparse;

void print_vcd(const VcdData& vcd_data);
void print_header(const Header& header);
void print_vars(const VcdData& vcd_data);
void print_changes(const VcdData& vcd_data);

int main(int argc, char** argv) {

    if(argc != 2) {
        std::cout << "Usage: " << argv[0] << " vcd_file" << "\n";
        return 1;
    }

    vcdparse::Loader vcd_loader;
    bool loaded = vcd_loader.load(argv[1]);
    if(loaded) {
        std::cout << "Successfully loaded VCD\n";

        auto& vcd_data = vcd_loader.get_vcd_data();
        //print_vcd(vcd_data);
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
    for(const auto& sigvals : vcd_data.signal_values()) {
        const auto& var = sigvals.var();
        std::cout << "$var ";
        std::cout << var.type() << " ";
        std::cout << var.width() << " ";
        std::cout << var.id() << " ";
        std::cout << var.name() << " ";
        std::cout << "$end\n";
    }
    std::cout << "$enddefinitions $end\n";
}

void print_changes(const VcdData& vcd_data) {
    //Collect all the changes by time
    std::map<size_t,std::vector<std::tuple<LogicValue,std::string>>> changes_by_time;
    for(const auto& sigvals : vcd_data.signal_values()) {
        const auto& var = sigvals.var();
        for(const auto& tv : sigvals.time_values()) {
            changes_by_time[tv.time()].push_back(std::make_tuple(tv.value(), var.id()));
        }
    }

    //Print them out
    for(auto kv : changes_by_time) {
        size_t time = kv.first;

        std::cout << "#" << time << "\n";

        for(auto change : kv.second) {
            LogicValue val;
            std::string id;
            std::tie(val, id) = change;
            std::cout << val << id << "\n";
        }
    }

}
