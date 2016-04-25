#pragma once

#include <iosfwd>
#include <memory>

#include "vcd_data.hpp"

namespace vcdparse {

//Forward delcarations
class Lexer;
class Parser;
class ParseError;

//Class for loading an SDF file.
//
//The vcd file can be parsed using load(), which returns true
//if successful - after which the loaded data can be accessed via 
//get_vcd_data().
//
//The virtual method on_error() can be overriding to control
//error handling. The default simply prints out an error message,
//but it could also be defined to (re-)throw an exception.
class Loader {

    public:
        Loader();
        ~Loader();

        bool load(std::string filename);
        bool load(std::istream& is, std::string filename="<inputstream>");

        const VcdData& get_vcd_data() { return vcd_data_; };

    protected:
        virtual void on_error(ParseError& error);

    private:
        friend Parser;
        std::string filename_;
        std::unique_ptr<Lexer> lexer_;
        std::unique_ptr<Parser> parser_;

        VcdData vcd_data_;
};

} //vcdparse
