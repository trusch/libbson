libbson
=======

A library for converting from and to BSON. Also contains a JSON parser/generator based on jsmn.

Its only export is a single class: BSON::Value. This class can hold any value you want to express in bson.
You can assign and reference its content by either by operator= or by the index operator.

A typical example would be this:


    BSON::Value doc = BSON::Object{
    	{"undefined",BSON::Value{}},
    	{"int32",(BSON::int32)1},
    	{"int64",(BSON::int64)1},
    	{"double",3.14},
    	{"true",true},
    	{"false",false},
    	{"string","foobar"},
    	{"datetime",std::chrono::milliseconds{123}},
    	{"object",BSON::Object{{"foo","bar"}}},
    	{"array",BSON::Array{1,2,3}}
    }; 

    std::string str = doc.toBSON();
    BSON::Value reDoc = BSON::Value::fromBSON(str);

    std::cout<<doc.toJSON()<<std::endl;
    std::cout<<reDoc.toJSON()<<std::endl; 
