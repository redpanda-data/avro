/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "avro/Schema.hh"
#include "Compiler.hh"

#include <boost/test/included/unit_test.hpp>
#include <boost/test/unit_test.hpp>
#include <sstream>

static const std::vector<std::string> validSimpleNames = {
    "validName",
    "_underscore",
    "name123",
    "camelCase"
};

static const std::vector<std::string> invalidSimpleNames = {
    "1invalid",      // starting with digit
    "invalid name",  // contains space
    "invalid-name",  // contains hyphen
    "invalid.name",  // contains period
    "invalid.namé",  // contains non-ASCII character
    ""               // empty name
};

static const std::vector<std::string> validSchemaNames = {
    "ValidName",
    "_underscore",
    "name123", 
    "CamelCase",
    "com.example.ValidName"
};

static const std::vector<std::string> invalidSchemaNames = {
    "1InvalidName", // starting with digit
    "Invalid Name", // contains space
    "Invalid-Name", // contains hyphen
    "com.example.1Invalid", // invalid full name
    "Invalid-Namé", // contains non-ASCII character
    "" // empty name
};

void testValidSchemaNames() {
    for (const auto& name : validSchemaNames) {
        BOOST_TEST_MESSAGE("Testing valid schema name: " << name);
        BOOST_CHECK_NO_THROW(avro::RecordSchema{name});
        BOOST_CHECK_NO_THROW(avro::EnumSchema{name});
    }
}

void testInvalidSchemaNames() {
    for (const auto& name : invalidSchemaNames) {
        BOOST_TEST_MESSAGE("Testing invalid schema name: " << name);
        BOOST_CHECK_THROW(avro::RecordSchema{name}, avro::Exception);
        BOOST_CHECK_THROW(avro::EnumSchema{name}, avro::Exception);
    }
}

void testValidFieldNames() {
    // Test valid field names
    for (const auto& name : validSimpleNames) {
        avro::RecordSchema record("TestRecord");
        BOOST_TEST_MESSAGE("Testing valid field name: " << name);
        BOOST_CHECK_NO_THROW(record.addField(name, avro::StringSchema()));
    }
}

void testInvalidFieldNames() {
    // Test invalid field names
    for (const auto& name : invalidSimpleNames) {
        avro::RecordSchema record("TestRecord");
        BOOST_TEST_MESSAGE("Testing invalid field name: " << name);
        BOOST_CHECK_THROW(record.addField(name, avro::StringSchema()), avro::Exception);
    }
}

void testValidEnumSymbols() {
    // Test valid enum symbols
    for (const auto& symbol : validSimpleNames) {
        avro::EnumSchema enumSchema("TestEnum");
        BOOST_TEST_MESSAGE("Testing valid enum symbol: " << symbol);
        BOOST_CHECK_NO_THROW(enumSchema.addSymbol(symbol));
    }
}

void testInvalidEnumSymbols() {
    // Test invalid enum symbols
    for (const auto& symbol : invalidSimpleNames) {
        avro::EnumSchema enumSchema("TestEnum");
        BOOST_TEST_MESSAGE("Testing invalid enum symbol: " << symbol);
        BOOST_CHECK_THROW(enumSchema.addSymbol(symbol), avro::Exception);
    }
}

void testValidSchemaNameFromJson() {
    // Test valid record names from JSON compilation    
    for (const auto& name : validSchemaNames) {
        BOOST_TEST_MESSAGE("(JSON) Testing valid schema name: " << name);
        
        // Test record schema
        std::string recordJson = R"({
            "type": "record",
            "name": ")" + name + R"(",
            "fields": [
                {"name": "testField", "type": "string"}
            ]
        })";
        BOOST_CHECK_NO_THROW(avro::compileJsonSchemaFromString(recordJson));
        
        // Test enum schema  
        std::string enumJson = R"({
            "type": "enum",
            "name": ")" + name + R"(",
            "symbols": ["A", "B", "C"]
        })";
        BOOST_CHECK_NO_THROW(avro::compileJsonSchemaFromString(enumJson));
    }
}

void testInvalidSchemaNameFromJson() {
    // Test invalid record names from JSON compilation
    for (const auto& name : invalidSchemaNames) {
        BOOST_TEST_MESSAGE("(JSON) Testing invalid schema name: " << name);
        
        // Test record schema
        std::string recordJson = R"({
            "type": "record",
            "name": ")" + name + R"(",
            "fields": [
                {"name": "testField", "type": "string"}
            ]
        })";
        BOOST_CHECK_THROW(avro::compileJsonSchemaFromString(recordJson), avro::Exception);
        
        // Test enum schema
        std::string enumJson = R"({
            "type": "enum", 
            "name": ")" + name + R"(",
            "symbols": ["A", "B", "C"]
        })";
        BOOST_CHECK_THROW(avro::compileJsonSchemaFromString(enumJson), avro::Exception);
    }
}

void testValidFieldNamesFromJson() {
    // Test valid field names from JSON compilation
    for (const auto& name : validSimpleNames) {
        BOOST_TEST_MESSAGE("(JSON) Testing valid field name: " << name);
        
        std::string recordJson = R"({
            "type": "record",
            "name": "TestRecord",
            "fields": [
                {"name": ")" + name + R"(", "type": "string"}
            ]
        })";
        BOOST_CHECK_NO_THROW(avro::compileJsonSchemaFromString(recordJson));
    }
}

void testInvalidFieldNamesFromJson() {
    // Test invalid field names from JSON compilation
    for (const auto& name : invalidSimpleNames) {
        BOOST_TEST_MESSAGE("(JSON) Testing invalid field name: " << name);
        
        std::string recordJson = R"({
            "type": "record",
            "name": "TestRecord", 
            "fields": [
                {"name": ")" + name + R"(", "type": "string"}
            ]
        })";
        BOOST_CHECK_THROW(avro::compileJsonSchemaFromString(recordJson), avro::Exception);
    }
}

void testValidEnumSymbolsFromJson() {
    // Test valid enum symbols from JSON compilation
    for (const auto& symbol : validSimpleNames) {
        BOOST_TEST_MESSAGE("(JSON) Testing valid enum symbol: " << symbol);
        
        std::string enumJson = R"({
            "type": "enum",
            "name": "TestEnum",
            "symbols": [")" + symbol + R"("]
        })";
        BOOST_CHECK_NO_THROW(avro::compileJsonSchemaFromString(enumJson));
    }
}

void testInvalidEnumSymbolsFromJson() {
    // Test invalid enum symbols from JSON compilation
    for (const auto& symbol : invalidSimpleNames) {
        BOOST_TEST_MESSAGE("(JSON) Testing invalid enum symbol: " << symbol);
        
        std::string enumJson = R"({
            "type": "enum",
            "name": "TestEnum",
            "symbols": [")" + symbol + R"("]
        })";
        BOOST_CHECK_THROW(avro::compileJsonSchemaFromString(enumJson), avro::Exception);
    }
}

boost::unit_test::test_suite *
init_unit_test_suite(int /*argc*/, char * /*argv*/[]) {
    using namespace boost::unit_test;

    auto *ts = BOOST_TEST_SUITE("Avro C++ name validation tests");
    
    // API-based validation tests
    ts->add(BOOST_TEST_CASE(&testValidSchemaNames));
    ts->add(BOOST_TEST_CASE(&testInvalidSchemaNames));
    ts->add(BOOST_TEST_CASE(&testValidFieldNames));
    ts->add(BOOST_TEST_CASE(&testInvalidFieldNames));
    ts->add(BOOST_TEST_CASE(&testValidEnumSymbols));
    ts->add(BOOST_TEST_CASE(&testInvalidEnumSymbols));
    
    // JSON compilation validation tests
    ts->add(BOOST_TEST_CASE(&testValidSchemaNameFromJson));
    ts->add(BOOST_TEST_CASE(&testInvalidSchemaNameFromJson));
    ts->add(BOOST_TEST_CASE(&testValidFieldNamesFromJson));
    ts->add(BOOST_TEST_CASE(&testInvalidFieldNamesFromJson));
    ts->add(BOOST_TEST_CASE(&testValidEnumSymbolsFromJson));
    ts->add(BOOST_TEST_CASE(&testInvalidEnumSymbolsFromJson));
    
    return ts;
}