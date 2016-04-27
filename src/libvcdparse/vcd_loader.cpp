#include <fstream>
#include "vcd_loader.hpp"

#include "vcd_lexer.hpp"
#include "vcd_parser.gen.hpp"
#include "location.hh"

namespace vcdparse {

Loader::Loader()
    : filename_("") //Initialize the filename
    , lexer_(new Lexer())
    , parser_(new Parser(*lexer_, *this)) {
}


//We need to declare the destructor in the .cpp file,
//since to destruct the unique_ptr's we also need the
//full definitions of Lexer and Parser
Loader::~Loader()
    {}

bool Loader::load(std::string filename) {
    std::ifstream is(filename);
    return load(is, filename);
}

bool Loader::load(std::istream& is, std::string filename) {
    assert(is.good());

    //Update the filename for location references
    filename_ = filename;

    //Point the lexer at the new input
    lexer_->switch_streams(&is);

    //Initialize locations with filename
    auto pos = position(&filename_);
    auto loc = location(pos, pos);
    lexer_->set_loc(loc);

    int retval;
    try {
        //Do the parsing
        retval = parser_->parse();

    } catch (ParseError& error) {
        //Users can re-define on_error if they want
        //to do something else (like re-throw)
        on_error(error);
        return false;
    }

    assert(verify_times());

    //Bision returns 0 if successful
    return (retval == 0);
}

void Loader::on_error(ParseError& error) {
    //Default implementation, just print out the error
    std::cout << "VCD Error " << error.loc() << ": " << error.what() << "\n";
}

bool Loader::verify_times() {
    for(const auto& sig_values : vcd_data_.signal_values()) {
        for(auto tv : sig_values.time_values()) {

            if(seen_times_.count(tv.time()) == 0) {
                assert(false);
                return false;
            }

        }
    }
    return true;
}

} //vcdparse
