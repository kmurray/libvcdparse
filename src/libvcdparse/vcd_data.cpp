#include "vcd_data.hpp"
#include <iostream>

namespace vcdparse {
    std::ostream& operator<<(std::ostream& os, LogicValue val) {
        if(val == LogicValue::ONE) os << "1";
        else if(val == LogicValue::ZERO) os << "0";
        else if(val == LogicValue::UNKOWN) os << "X";
        else if(val == LogicValue::HIGHZ) os << "Z";
        else assert(false);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, Var::Type type) {
        if(type == Var::Type::WIRE) os << "WIRE";
        else if(type == Var::Type::REG) os << "REG";
        else if(type == Var::Type::PARAMETER) os << "PARAMETER";
        else assert(false);
        return os;
    }
} //vcdparse
