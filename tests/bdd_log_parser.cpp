#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include "LogParser.h"
#include "DataTypes.h"

// Simple BDD Testing Framework
namespace BDD {
    struct TestContext {
        std::string current_scenario;
        int assertions = 0;
        int failures = 0;
    };

    TestContext context;

    void SCENARIO(const std::string& name) {
        if (!context.current_scenario.empty()) {
            std::cout << context.assertions << " assertions, " << context.failures << " failures." << std::endl;
        }
        std::cout << "\nScenario: " << name << std::endl;
        context.current_scenario = name;
        context.assertions = 0;
        context.failures = 0;
    }

    void GIVEN(const std::string& given) {
        std::cout << "  Given: " << given << std::endl;
    }

    void WHEN(const std::string& when) {
        std::cout << "  When: " << when << std::endl;
    }

    void THEN(const std::string& then) {
        std::cout << "  Then: " << then << std::endl;
    }

    template<typename T>
    void EXPECT_EQ(const T& actual, const T& expected, const std::string& message) {
        context.assertions++;
        if (actual == expected) {
            std::cout << "    \033[32m[PASS]\033[0m " << message << std::endl;
        } else {
            context.failures++;
            std::cout << "    \033[31m[FAIL]\033[0m " << message << std::endl;
            std::cout << "      Expected: " << expected << ", Actual: " << actual << std::endl;
        }
    }

    void EXPECT_TRUE(bool condition, const std::string& message) {
        context.assertions++;
        if (condition) {
            std::cout << "    \033[32m[PASS]\033[0m " << message << std::endl;
        } else {
            context.failures++;
            std::cout << "    \033[31m[FAIL]\033[0m " << message << std::endl;
        }
    }

    void summary() {
        if (!context.current_scenario.empty()) {
            std::cout << context.assertions << " assertions, " << context.failures << " failures." << std::endl;
        }
    }
}

// Helper function to create a temporary test log file
void createTestLogFile(const std::string& filename, const std::string& content) {
    std::ofstream outFile(filename);
    outFile << content;
    outFile.close();
}

void test_log_parser_with_valid_file() {
    BDD::SCENARIO("Parsing a valid log file");

    const std::string filename = "bdd_valid_log.csv";
    const std::string content = "timestamp,responseId,latency,tokensUsed,response\n"
                                "2023-10-27T10:00:00Z,resp-001,0.5,150,\"A valid response.\"\n"
                                "2023-10-27T10:00:05Z,resp-002,0.7,200,\"Another one.\"\n";

    BDD::GIVEN("a log file with valid entries");
    createTestLogFile(filename, content);

    BDD::WHEN("the log file is parsed");
    auto entries = LogParser::parseLogFile(filename);

    BDD::THEN("it should return the correct number of log entries");
    BDD::EXPECT_EQ(entries.size(), (size_t)2, "Correct number of entries");

    BDD::THEN("the log entry data should be correct");
    if (entries.size() == 2) {
        BDD::EXPECT_EQ(entries[0].responseId, std::string("resp-001"), "Response ID of first entry is correct");
        BDD::EXPECT_EQ(entries[0].latency, 0.5, "Latency of first entry is correct");
        BDD::EXPECT_EQ(entries[0].tokensUsed, 150, "Tokens used of first entry is correct");
        BDD::EXPECT_EQ(entries[0].response, std::string("A valid response."), "Response text of first entry is correct");
    }

    remove(filename.c_str());
}

void test_log_parser_with_empty_file() {
    BDD::SCENARIO("Parsing an empty log file");

    const std::string filename = "bdd_empty_log.csv";

    BDD::GIVEN("an empty log file");
    createTestLogFile(filename, "");

    BDD::WHEN("the log file is parsed");
    auto entries = LogParser::parseLogFile(filename);

    BDD::THEN("it should return no log entries");
    BDD::EXPECT_TRUE(entries.empty(), "The log entries vector is empty");

    remove(filename.c_str());
}

void test_log_parser_with_header_only_file() {
    BDD::SCENARIO("Parsing a log file with only a header");

    const std::string filename = "bdd_header_only_log.csv";
    const std::string content = "timestamp,responseId,latency,tokensUsed,response\n";

    BDD::GIVEN("a log file with only a header");
    createTestLogFile(filename, content);

    BDD::WHEN("the log file is parsed");
    auto entries = LogParser::parseLogFile(filename);

    BDD::THEN("it should return no log entries");
    BDD::EXPECT_TRUE(entries.empty(), "The log entries vector is empty");

    remove(filename.c_str());
}


int main() {
    test_log_parser_with_valid_file();
    test_log_parser_with_empty_file();
    test_log_parser_with_header_only_file();
    BDD::summary();
    return 0;
}
