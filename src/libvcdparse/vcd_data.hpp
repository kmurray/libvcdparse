#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>

#include "vcd_data_fwd.hpp"

//The classes defined in this file correspond to the data loaded from the VCD file

namespace vcdparse {

    enum class LogicValue {
        ONE,
        ZERO,
        UNKOWN,
        HIGHZ
    };

    class TimeValue {
            TimeValue(size_t new_time, LogicValue new_value)
                : time_(new_time)
                , value_(new_value)
                {}

            size_t time() const { return time_; }
            LogicValue value() const { return value_; }
        
        public:
            size_t time_;
            LogicValue value_;
    };

    class SignalValues {
        public:
            typedef std::vector<TimeValue> TimeValues;

            SignalValues(const std::string& new_hierarchical_name, const std::string& new_id, const TimeValues& tvs)
                : hierarchical_name_(new_hierarchical_name)
                , id_(new_id)
                , time_values_(tvs)
                {}

            const std::string& heirarchical_name() const { return hierarchical_name_; }
            std::string name() const { 
                //We use / as the hierarchy divider
                auto idx = hierarchical_name_.find_last_of("/");
                if(idx == std::string::npos) {
                    //No heirarhcy in name
                    return hierarchical_name_; 
                } else {
                    assert(idx + 1 < hierarchical_name_.size());
                    //Split of only the end of the name
                    return std::string(hierarchical_name_.begin() + idx + 1, hierarchical_name_.end());
                }
            }
            const std::string& id() const { return id_; }
            const TimeValues& time_values() const { return time_values_; }

            void add_time_value(const TimeValue& tv) { time_values_.push_back(tv); }

        private:
            std::string hierarchical_name_;
            std::string id_; //i.e. symbol used to represent this signal in the VCD
            TimeValues time_values_;
    };

    class Header {
        public:
            const std::string& date() const { return date_; }
            const std::string& version() const { return version_; }
            const std::string& timescale() const { return timescale_; }

            void set_date(const std::string& new_date) { date_ = new_date; }
            void set_version(const std::string& new_version) { version_ = new_version; }
            void set_timescale(const std::string& new_timescale) { timescale_ = new_timescale; }
        private:
            std::string date_;
            std::string version_;
            std::string timescale_;
    };


    class VcdData {
        public:
            const Header& header() { return header_; }
            void set_header(const Header& new_header) { header_ = new_header; }

            void add_signal_values(const std::string& key, const SignalValues& values) { 
                auto ret = signal_values_.insert(std::make_pair(key, values));
                assert(ret.second); //Was inserted
            }

            const SignalValues& get_signal_values(const std::string& name) { 
                auto iter = signal_values_.find(name);
                assert(iter != signal_values_.end());

                return iter->second;
            }

        private:
            Header header_;
            std::unordered_map<std::string, SignalValues> signal_values_;

    };

} //namespace vcdparse
