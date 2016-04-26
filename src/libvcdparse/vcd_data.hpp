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
        public:
            TimeValue(size_t new_time, LogicValue new_value)
                : time_(new_time)
                , value_(new_value)
                {}

            size_t time() const { return time_; }
            LogicValue value() const { return value_; }
        
        private:
            size_t time_;
            LogicValue value_;
    };

    class Var {
        public:
            enum class Type {
                WIRE,
                REG,
                PARAMETER
            };

            Var() = default;
            Var(Type new_type, size_t new_width, char new_id, std::vector<std::string> new_hierarchical_name)
                : type_(new_type)
                , width_(new_width)
                , id_(new_id)
                , hierarchical_name_(new_hierarchical_name)
                {}

            Type type() { return type_; }
            size_t width() { return width_; }
            char id() { return id_; }
            std::vector<std::string> hierarchical_name() { return hierarchical_name_; }
            std::string name() { return *(--hierarchical_name_.end()); }

        private:
            Type type_;
            size_t width_;
            char id_;
            std::vector<std::string> hierarchical_name_;
    };

    class SignalValues {
        public:
            typedef std::vector<TimeValue> TimeValues;

            SignalValues(const Var& new_var, const TimeValues& tvs)
                : var_(new_var)
                , time_values_(tvs)
                {}

            const TimeValues& time_values() const { return time_values_; }
            const Var& var() const { return var_; }

        private:
            Var var_;
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
            typedef std::unordered_map<char, std::vector<TimeValue>> TimeValuesById;
            typedef std::unordered_map<char, std::string> NamesById;
            VcdData() = default;
            VcdData(const Header& new_header, std::vector<SignalValues> new_signal_values)
                : header_(new_header)
                , signal_values_(new_signal_values)
                {}

            const Header& header() { return header_; }

        private:
            Header header_;
            std::vector<SignalValues> signal_values_;

    };

} //namespace vcdparse
