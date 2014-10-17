/*
 * Copyright (c) 2014, Tino Rusch
 *
 * This file is released under the terms of the MIT license. You can find the
 * complete text in the attached LICENSE file or online at:
 *
 * http://www.opensource.org/licenses/mit-license.php
 * 
 * @author: Tino Rusch (tino.rusch@webvariants.de)
 */

#ifndef __BSON_VALUE__
#define __BSON_VALUE__

#include <string>
#include <stdint.h>
#include <chrono>
#include <stdexcept>
#include <vector>
#include <map>

#include <iostream>
#include <sstream>


#define JSON_TOKENS (1<<12)

namespace BSON {
	
	class Value;

	typedef int32_t   int32;
	typedef long long int64;
	typedef std::map<std::string,Value> Object;
	typedef std::vector<Value> Array;

	enum Type {
		UNDEFINED,
		INT32,
		INT64,
		DOUBLE,
		BOOL,
		STRING,
		BINARY,
		DATETIME,
		OBJECT,
		ARRAY
	};

	class Value {
	protected:
		enum Type _type;	
		int32                          _int32Value;
		int64                          _int64Value;
		double                         _doubleValue;
		bool                           _boolValue;
		std::string                    _stringValue;
		std::chrono::milliseconds      _datetimeValue;

		std::map<std::string,Value>    _objectValue;
		std::vector<Value>             _arrayValue;

		void checkType(enum Type type) const;

		//BSON Helper
		std::string getTypePrefix() const;

	public:
		Value() : _type{UNDEFINED} {}
		Value(const Value & val);
		Value(const int32 & val) : _type{INT32}, _int32Value{val} {}
		Value(const short & val) : Value{int32{val}} {}
		Value(const int64 & val) : _type{INT64}, _int64Value{val} {}
		Value(const long & val) : Value{int64{val}} {}
		Value(const double & val) : _type{DOUBLE}, _doubleValue{val} {}
		Value(const bool & val) : _type{BOOL}, _boolValue{val} {}
		Value(const std::string & val) : _type{STRING}, _stringValue{val} {}
		Value(const char * val) : _type{STRING}, _stringValue{val} {}
		Value(char* data, size_t len) : _type{BINARY}, _stringValue{data,len} {}
		Value(const std::chrono::milliseconds & val) : _type{DATETIME}, _datetimeValue{val} {}
		Value(const std::map<std::string,Value> & val) : _type{OBJECT}, _objectValue{val} {}
		Value(const std::vector<Value> & val) : _type{ARRAY}, _arrayValue{val.begin(),val.end()} {}

		operator int32 &() {checkType(INT32); return _int32Value;}
		operator int64 &() {checkType(INT64); return _int64Value;}
		operator double &() {checkType(DOUBLE); return _doubleValue;}
		operator bool &() {checkType(BOOL); return _boolValue;}
		operator std::string &() {checkType(STRING); return _stringValue;}
		operator char*() {checkType(BINARY); return (char*)_stringValue.c_str();}
		operator std::chrono::milliseconds &() {checkType(DATETIME); return _datetimeValue;}
		operator std::map<std::string,Value> &() {checkType(OBJECT); return _objectValue;}
		operator std::vector<Value> &() {checkType(ARRAY); return _arrayValue;}

		Value& operator=(const int32 & val){checkType(INT32); _type=INT32; _int32Value=val; return *this;}
		Value& operator=(const int64 & val){checkType(INT64); _type=INT64; _int64Value=val; return *this;}
		Value& operator=(const double & val){checkType(DOUBLE); _type=DOUBLE; _doubleValue=val; return *this;}
		Value& operator=(const bool & val){checkType(BOOL); _type=BOOL; _boolValue=val; return *this;}
		Value& operator=(const std::string & val){checkType(STRING); _type=STRING; _stringValue=val; return *this;}
		Value& operator=(const char * val){checkType(STRING); _type=STRING; _stringValue=std::string{val}; return *this;}
		Value& operator=(const std::chrono::milliseconds & val){checkType(DATETIME); _type=DATETIME; _datetimeValue=val; return *this;}
		Value& operator=(const std::map<std::string,Value> & val){checkType(OBJECT); _type=OBJECT; _objectValue=val; return *this;}
		Value& operator=(const std::vector<Value> & val){checkType(ARRAY); _type=ARRAY; _arrayValue=val; return *this;}
		
		bool operator==(const Value & other) const;
		bool operator!=(const Value & other) const {return !(*this==other);}

		Value& operator[](const std::string & key);
		Value& operator[](const char * key);
		Value& operator[](const int & index);

		const Value& operator[](const std::string & key) const;
		const Value& operator[](const char * key) const;
		const Value& operator[](const int & index) const;

		std::map<std::string,Value>::iterator begin();
		std::map<std::string,Value>::iterator end();

		std::map<std::string,Value>::const_iterator cbegin() const;
		std::map<std::string,Value>::const_iterator cend() const;

		void push_back(const Value & val);
		void pop_back();

		inline void reset(){_type = UNDEFINED;}
		inline void setType(enum Type type){_type = type;}
		inline enum Type getType() const {return _type;}
		size_t size() const;

		//JSON functions
		std::string toJSON() const;
		static Value fromJSON(const std::string & json);

		//BSON functions
		std::string toBSON() const;
		static Value fromBSON(const std::string & bson);
	};
}

#endif // __BSON_VALUE__
