#include "Value.h"
#include "jsmn.h"


namespace BSON {
    bool isInteger(const std::string & s) {
        if( s.empty() || ( ( !isdigit( s[0] ) ) && ( s[0] != '-' ) && ( s[0] != '+' ) ) ) return false ;
        char * p ;
        strtol( s.c_str(), &p, 10 ) ;
        return ( *p == 0 ) ;
    }

    bool isDouble(const std::string & s) {
        std::stringstream ss( s );
        double d;
        return ( ss >> d ) && ( ss >> std::ws ).eof();
    }

    bool isJsonPrimitive( const std::string & str ) {
        if( str.find( "{" ) != std::string::npos ||
                str.find( "[" ) != std::string::npos ||
                str.find( "\"" ) != std::string::npos ) {
            return false;
        }
        return true;
    }

    std::string escapeJSON( const std::string& input ) {
        std::string output;
        output.reserve( input.length() );

        for( std::string::size_type i = 0; i < input.length(); ++i )
        {
            switch( input[i] ) {
                case '"':
                    output += "\\\"";
                    break;
                case '/':
                    output += "\\/";
                    break;
                case '\b':
                    output += "\\b";
                    break;
                case '\f':
                    output += "\\f";
                    break;
                case '\n':
                    output += "\\n";
                    break;
                case '\r':
                    output += "\\r";
                    break;
                case '\t':
                    output += "\\t";
                    break;
                case '\\':
                    output += "\\\\";
                    break;
                default:
                    output += input[i];
                    break;
            }
        }
        return output;
    }

    std::string unescapeJSON(const std::string& input) {
        enum State {
            ESCAPED,
            UNESCAPED
        };
        State s = UNESCAPED;
        std::string output;
        output.reserve(input.length());

        for (std::string::size_type i = 0; i < input.length(); ++i)
        {
            switch(s)
            {
                case ESCAPED:
                    {
                        switch(input[i])
                        {
                            case '"':
                                output += '\"';
                                break;
                            case '/':
                                output += '/';
                                break;
                            case 'b':
                                output += '\b';
                                break;
                            case 'f':
                                output += '\f';
                                break;
                            case 'n':
                                output += '\n';
                                break;
                            case 'r':
                                output += '\r';
                                break;
                            case 't':
                                output += '\t';
                                break;
                            case '\\':
                                output += '\\';
                                break;
                            default:
                                output += input[i];
                                break;
                        }

                        s = UNESCAPED;
                        break;
                    }
                case UNESCAPED:
                    {
                        switch(input[i])
                        {
                            case '\\':
                                s = ESCAPED;
                                break;
                            default:
                                output += input[i];
                                break;
                        }
                    }
            }
        }
        return output;
    }

    Value tokenToValue( jsmntok_t * & t, const  char *js ) {
        Value result;
        switch( t->type ) {
            case JSMN_OBJECT: {
                int objectSize = t->size;
                t++;
                result = Object {};
                for( int i=0; i<objectSize; i+=2 ) {
                    std::string key = tokenToValue( t,js );
                    Value value = tokenToValue( t,js );
                    result[key] = value;
                }
                break;
            }
            case JSMN_ARRAY: {
                int arraySize = t->size;
                t++;
                result = Array {};
                for( int i=0; i<arraySize; i++ ) {
                    Value value = tokenToValue( t,js );
                    result.push_back( value );
                }
                break;
            }
            case JSMN_STRING: {
                std::string value {js+( t->start ),js+( t->end )};
                result = unescapeJSON(value);
                t++;
                break;
            }
            case JSMN_PRIMITIVE: {
                std::string str {js+( t->start ),js+( t->end )};
                if( str.compare( "null" ) == 0 ) {
                    result = Value {};
                }
                else if( str.compare( "true" ) == 0 ) {
                    result = true;
                }
                else if( str.compare( "false" ) == 0 ) {
                    result = false;
                }
                else if( str.find( '.' ) != std::string::npos ) {
                    result = std::stod( str );
                }
                else {
                    result = std::stoll( str );
                }
                t++;
                break;
            }
        }
        return result;
    }




    


    Value Value::fromJSON(const std::string & str){
        try {
            if( isJsonPrimitive( str ) ) {
                Value v;
                if( isInteger( str ) ) {
                    v = std::stoi( str );
                }
                else if( isDouble( str ) ) {
                    v = std::stod( str );
                }
                else if( str=="true" ) {
                    v = true;
                }
                else if( str=="false" ) {
                    v = false;
                }
                else {
                    //value is String
                    v = unescapeJSON(str);
                }

                return v;
            }
            else {

                int r;
                jsmn_parser p;
                jsmntok_t tokens[JSON_TOKENS];
                jsmn_init( &p );
                r = jsmn_parse( &p, str.c_str(), str.size(), tokens, JSON_TOKENS );

                if( r < 0 ) {
                    throw std::runtime_error {"Value::fromJSONString parse error"};
                }

                jsmntok_t * start = &tokens[0];
                return tokenToValue( start,str.c_str() );
            }
        }
        catch( ... ) {
            return Value {};
        }
    }

    std::string Value::toJSON() {
        std::string result;
        switch(_type){
            case UNDEFINED:
                result = "null";
                break;
            case INT32:
                result = std::to_string(_int32Value);
                break;
            case INT64:
                result = std::to_string(_int64Value);
                break;
            case DOUBLE:
                result = std::to_string(_doubleValue);
                break;
            case BOOL:
                result = _boolValue ? "true" : "false";
                break;
            case STRING:
                result = "\""+escapeJSON(_stringValue)+"\"";
                break;
            case BINARY:
                throw std::runtime_error{"BINARY data type is not representable in json"};
                break;
            case DATETIME:
                result = std::to_string(_datetimeValue);
                break;
            case ARRAY:
                result = "[";
                if( this->size() )for( size_t i = 0; i <= size()-1; ++i ) {
                    result += _arrayValue[i].toJSON();
                    if( i < size()-1 ) {
                        result += ",";
                    }
                }
                result += "]";
                break;
            case OBJECT:
                result += "{";
                size_t current = 0;
                for( auto & kv : _objectValue ) {
                    result += "\"";
                    result += escapeJSON( kv.first );
                    result += "\":";
                    result += kv.second.toJSON();
                    if( current++ < size()-1 ) {
                        result += ",";
                    }
                }
                result += "}";
                break;
        }
        return result;
    }
}
