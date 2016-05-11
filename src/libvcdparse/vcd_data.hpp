#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <limits>

#include "vcd_data_fwd.hpp"

//The classes defined in this file correspond to the data loaded from the VCD file

namespace vcdparse {

    enum class LogicValue {
        ONE,
        ZERO,
        UNKOWN,
        HIGHZ
    };
    std::ostream& operator<<(std::ostream& os, LogicValue val);

    class TimeValue {
        public:
            TimeValue(size_t new_time=0, LogicValue new_value=LogicValue::UNKOWN)
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
            Var(Type new_type, size_t new_width, std::string new_id, std::vector<std::string> new_hierarchical_name)
                : type_(new_type)
                , width_(new_width)
                , id_(new_id)
                , hierarchical_name_(new_hierarchical_name)
                {}

            Type type() const { return type_; }
            size_t width() const { return width_; }
            std::string id() const { return id_; }
            std::vector<std::string> hierarchical_name() const { return hierarchical_name_; }
            std::string name() const { return *(--hierarchical_name_.end()); }

        private:
            Type type_;
            size_t width_;
            std::string id_;
            std::vector<std::string> hierarchical_name_;
    };
    std::ostream& operator<<(std::ostream& os, Var::Type type);

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
            VcdData() = default;
            VcdData(const Header& new_header, std::vector<SignalValues> new_signal_values)
                : header_(new_header)
                , signal_values_(std::move(new_signal_values))
                {}

            const Header& header() const { return header_; }
            const std::vector<SignalValues>& signal_values() const { return signal_values_; }

        private:
            Header header_;
            std::vector<SignalValues> signal_values_;

    };

} //namespace vcdparse
