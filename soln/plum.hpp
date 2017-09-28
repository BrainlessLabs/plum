#pragma once

#include <cstdint>
#include <map>
#include <msgpack.hpp>
#include <string>
#include <iostream>
#include <sstream>

namespace plum {
	enum DataType {
		kUnsignedInteger = 0,
		kInteger,
		kFloat,
		kDouble,
		kString,
		kJson
	};

	union DataHolder {
		std::uint64_t ui;
		std::int64_t i;
		float f;
		double d;
		char* cp;
	};

	struct Data {
		DataType data_type;
		DataHolder data;
	};

	class Storage {
	private:
		std::map<std::string, Data> _data_hash;

	public:
		Storage() = default;
	};
}
