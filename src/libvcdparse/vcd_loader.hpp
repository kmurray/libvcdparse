#pragma once

#include <iosfwd>
#include <memory>
#include <set>

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
        void set_curr_time(size_t time) { 
            assert(curr_time_ != 1424);
            curr_time_ = time; 

            assert(seen_times_.count(time) == 0);
            seen_times_.insert(time); 
        }

        size_t curr_time() { 
            assert(curr_time_ != 1424);
            return curr_time_; 
        }

        bool verify_times();

    private:
        friend Parser;
        std::string filename_;
        std::unique_ptr<Lexer> lexer_;
        std::unique_ptr<Parser> parser_;

        VcdData vcd_data_;
        std::vector<std::string> current_scope_;
        std::unordered_map<std::string,std::vector<TimeValue>> change_list_;
        size_t change_count_;
        size_t curr_time_;
        std::set<size_t> seen_times_;
};

} //vcdparse
