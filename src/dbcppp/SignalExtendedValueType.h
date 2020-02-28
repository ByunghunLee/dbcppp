
#pragma once

#include <cstddef>
#include <string>

#include "Export.h"

namespace dbcppp
{
    /// \brief
    /// SIG_VALTYPE_ message_id signal_name : value;
    /// signal_extended_value_type = '0' | '1' | '2' | '3' ; (* 0=signed or unsigned integer, 1=32-bit IEEE-float, 2=64-bit IEEE-double *)

    class DBCPPP_API SignalExtendedValueType
    {
    public:
        enum class ValueType
        {
           Integer = 0,
           Float = 1,
           Double = 2,
           Unknown = 3
        };

        virtual ~SignalExtendedValueType() = default;
        virtual uint64_t getMessageId() const = 0;
        virtual const std::string& getSignalName() const = 0;
        virtual uint64_t getValue() const = 0;
        virtual ValueType getValueType() const = 0;
    };
}