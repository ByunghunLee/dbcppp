
#pragma once

#include <string>
#include <cstddef>
#include <boost/variant.hpp>

#include "Export.h"
#include "AttributeDefinition.h"

namespace dbcppp
{
	class DBCPPP_API Attribute
	{
	public:
	    using value_t = boost::variant<int64_t, double, std::string>;

		virtual ~Attribute() = default;
		virtual const std::string& getName() const = 0;
		virtual AttributeDefinition::ObjectType getObjectType() const = 0;
		virtual const value_t& getValue() const = 0;
	};
}
