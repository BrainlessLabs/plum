#pragma once

#include <cstdint>
#include <map>
#define MSGPACK_USE_X3_PARSE
#include <msgpack.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <boost/core/noncopyable.hpp>
#include <spdlog/spdlog.h>

namespace plum {
	template <class T>
	class Singleton : public boost::noncopyable {
	public:
		static T& i() {
			static T _me;
			return _me;
		}
	};

	/////////////////////////////////////////////////
	/// @fn l
	/// @brief Returns the logger
	///        This will be logging to the logs. It uses spdlog to log to file
	/// @return Returns the logger.
	/////////////////////////////////////////////////
	inline spdlog::logger& l() {
		static const std::size_t q_size = 1048576; //queue size must be power of 2
		spdlog::set_async_mode(q_size);
		static auto ret = spdlog::daily_logger_st("async_file_logger", "query_log.txt");
		return *ret;
	}

	enum class Instructions : std::uint8_t {
		kUnknown = 0,
		kCreateDatabase,
		kDeleteDatabase,
		kAdd,
		kUpdate,
		kDelete
	};

	enum class DataType : std::uint32_t {
		kUnknown = 0,
		kUnsignedInteger,
		kInteger,
		kFloat,
		kDouble,
		kString,
		kBlob,
		kJson
	};

	union DataHolder {
		std::uint64_t ui;
		std::int64_t i;
		char c;
		unsigned char uc;
		float f;
		double d;
		char* cp;
		void* vp;
	};

	struct Data {
		DataType data_type;
		DataHolder data;
		std::size_t size;

		Data() = default;

		void set(const std::uint64_t ui) {
			data_type = DataType::kUnsignedInteger;
			data.ui = ui;
		}

		void set(const std::int64_t i) {
			data_type = DataType::kInteger;
			data.i = i;
		}

		void set(const float f) {
			data_type = DataType::kFloat;
			data.f = f;
		}

		void set(const double d) {
			data_type = DataType::kDouble;
			data.d = d;
		}

		void set(const char* cp) {
			data_type = DataType::kString;
			data.cp = const_cast<char*>(cp);
			size = std::strlen(data.cp);
		}

		void set(const std::string& s) {
			data_type = DataType::kString;
			data.cp = const_cast<char*>(s.c_str());
			size = s.length();
		}
	};

	class Database {
	public:
		typedef std::map<std::string, Data> DataMap;

	private:
		std::map<std::string, DataMap> _db_map;

	public:
		Database() = default;

		DataMap& create_db(const std::string db_name) {
			_db_map[db_name] = DataMap();
			return db(db_name);
		}

		bool delete_db(const std::string db_name) {
			bool ret = false;
			auto pos = _db_map.find(db_name);
			if (pos != _db_map.end()) {
				_db_map.erase(pos);
				ret = true;
			}
			return ret;
		}

		DataMap& db(const std::string db_name) {
			return _db_map[db_name];
		}

		bool exists(const std::string db_name) const {
			return _db_map.count(db_name) > 0 ? true : false;
		}
	};

	class PlumMain {
	private:
		Database db;

	public:
		PlumMain() {
			db.create_db("default");
		}
		~PlumMain() = default;
	};
}
