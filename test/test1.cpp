#include "gtest/gtest.h"
#include "Value.h"

TEST(BSON,Basic){
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

	std::string encoded1 = doc.toBSON();
	auto doc2 = BSON::Value::fromBSON(encoded1);
	std::string encoded2 = doc2.toBSON();

	EXPECT_EQ(encoded1,encoded2);
}

TEST(BSON, Equality){
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

	BSON::Value doc2 = 23;

	EXPECT_TRUE(doc==doc);
	EXPECT_FALSE(doc==doc2);
}

TEST(BSON, Construct){
	BSON::Value undefined;
	BSON::Value integer_32bit{(int32_t)1};
	BSON::Value integer_64bit{(long long)1};
	BSON::Value doubleVal{3.14};
	BSON::Value boolVal{true};
	BSON::Value stringVal{"foobar"};
	BSON::Value date{std::chrono::milliseconds{123}};
	BSON::Value object{BSON::Object{}};
	BSON::Value array{BSON::Array{}};

	EXPECT_EQ(BSON::UNDEFINED,undefined.getType());
	EXPECT_EQ(BSON::INT32,integer_32bit.getType());
	EXPECT_EQ(BSON::INT64,integer_64bit.getType());
	EXPECT_EQ(BSON::DOUBLE,doubleVal.getType());
	EXPECT_EQ(BSON::BOOL,boolVal.getType());
	EXPECT_EQ(BSON::STRING,stringVal.getType());
	EXPECT_EQ(BSON::DATETIME,date.getType());
	EXPECT_EQ(BSON::OBJECT,object.getType());
	EXPECT_EQ(BSON::ARRAY,array.getType());	
}