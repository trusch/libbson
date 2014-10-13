#include "Value.h"


BSON::Value::Value(const Value & val) : _type{val._type} {
	switch(_type){
		case BSON::UNDEFINED:
			break;
		case BSON::INT32:
			_int32Value = val._int32Value;
			break;
		case BSON::INT64:
			_int64Value = val._int64Value;
			break;
		case BSON::DOUBLE:
			_doubleValue = val._doubleValue;
			break;
		case BSON::BOOL:
			_boolValue = val._boolValue;
			break;
		case BSON::STRING:
			_stringValue = val._stringValue;
			break;
		case BSON::BINARY:
			_stringValue = val._stringValue;
			break;
		case BSON::DATETIME:
			_datetimeValue = val._datetimeValue;
			break;
		case BSON::OBJECT:
			_objectValue = val._objectValue;
			break;
		case BSON::ARRAY:
			_arrayValue = val._arrayValue;
			break;
	}
}

std::string BSON::Value::getType() const {
	switch(_type){
		case UNDEFINED:
			return "UNDEFINED";
			break;
		case INT32:
			return "INT32";
			break;
		case INT64:
			return "INT64";
			break;
		case DOUBLE:
			return "DOUBLE";
			break;
		case BOOL:
			return "BOOL";
			break;
		case STRING:
			return "STRING";
			break;
		case BINARY:
			return "BINARY";
			break;
		case DATETIME:
			return "DATETIME";
			break;
		case OBJECT:
			return "OBJECT";
			break;
		case ARRAY:
			return "ARRAY";
			break;
	}
}

BSON::Value& BSON::Value::operator[](const std::string & key){
	checkType(OBJECT);
	_type = OBJECT;
	return _objectValue[key];
}

BSON::Value& BSON::Value::operator[](const char * key){
	return operator[](std::string{key});
}

BSON::Value& BSON::Value::operator[](const int & index){
	checkType(ARRAY);
	_type = ARRAY;
	return _arrayValue[index];
}

void BSON::Value::push_back(const Value & val){
	checkType(ARRAY);
	_type = ARRAY;
	_arrayValue.push_back(val);	
}

void BSON::Value::pop_back(){
	checkType(ARRAY);
	_type = ARRAY;
	_arrayValue.pop_back();	
}

size_t BSON::Value::size(){
	if(_type == ARRAY) {
		return _arrayValue.size();
	}
	if(_type == OBJECT) {
		return _objectValue.size();
	}
	throw std::runtime_error{"wrong type"};
}

std::map<std::string,BSON::Value>::iterator BSON::Value::begin() {
	checkType(OBJECT);
	_type = OBJECT;
	return _objectValue.begin();
}

std::map<std::string,BSON::Value>::iterator BSON::Value::end() {
	checkType(OBJECT);
	_type = OBJECT;
	return _objectValue.end();
}

void BSON::Value::checkType(enum Type type) const {
	if(_type != UNDEFINED && _type != type){
		throw std::runtime_error{"wrong type"};
	}
}
