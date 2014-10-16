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

const BSON::Value& BSON::Value::operator[](const std::string & key) const{
	checkType(OBJECT);
	if(_type == UNDEFINED)throw std::runtime_error{"wrong type"};
	return _objectValue.at(key);
}

const BSON::Value& BSON::Value::operator[](const char * key) const{
	return operator[](std::string{key});
}

const BSON::Value& BSON::Value::operator[](const int & index) const{
	checkType(ARRAY);
	if(_type == UNDEFINED)throw std::runtime_error{"wrong type"};
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

size_t BSON::Value::size() const {
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

std::map<std::string,BSON::Value>::const_iterator BSON::Value::cbegin() const {
	checkType(OBJECT);
	if(_type == BSON::UNDEFINED)throw std::runtime_error{"wrong type"};
	return _objectValue.cbegin();
}

std::map<std::string,BSON::Value>::const_iterator BSON::Value::cend() const {
	checkType(OBJECT);
	if(_type == BSON::UNDEFINED)throw std::runtime_error{"wrong type"};
	return _objectValue.cend();
}

void BSON::Value::checkType(enum Type type) const {
	if(_type != UNDEFINED && _type != type){
		throw std::runtime_error{"wrong type"};
	}
}

bool BSON::Value::operator==(const Value & other) const{
	if(_type != other._type) return false;
	switch(_type){
		case UNDEFINED:
			return true;
		case INT32:
			return _int32Value == other._int32Value;
		case INT64:
			return _int64Value == other._int64Value;
		case DOUBLE:
			return _doubleValue == other._doubleValue;
		case BOOL:
			return _boolValue == other._boolValue;
		case STRING:
		case BINARY:
			return _stringValue == other._stringValue;
		case DATETIME:
			return _datetimeValue == other._datetimeValue;
		case OBJECT:
			if(size() != other.size()) return false;
			for(auto it = cbegin(); it!=cend(); ++it){
				const Value & left = (*this)[it->first];
				const Value & right = other[it->first];
				if(left != right){
					return false;
				}
			}
			return true;
		case ARRAY:
			if(size() != other.size()) return false;
			for(size_t i=0; i<size(); ++i){
				const Value & left = (*this)[i];
				const Value & right = other[i];
				if(left != right){
					return false;
				}
			}
			return true;
	}
}