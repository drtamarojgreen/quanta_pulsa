cmake_minimum_required(VERSION 3.10)
project(QuantaPulsa)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(QuantaPulsa
    src/QuantaPulsa.cpp
    src/LogParser.cpp
    src/CognitiveEngine.cpp
    src/TelemetryExporter.cpp
    src/LegacyMonitoring.cpp
)

target_include_directories(QuantaPulsa PRIVATE include)

# --- Google Test Setup ---
include(FetchContent)
enable_testing()

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG    release-1.12.1
)

FetchContent_MakeAvailable(googletest)

# --- Test Executable for LogParser ---
add_executable(
  runLogParserTests
  tests/test_log_parser.cpp
  src/LogParser.cpp
)

target_include_directories(runLogParserTests PRIVATE include)

target_link_libraries(
  runLogParserTests
  gtest_main
)

include(GoogleTest)
gtest_add_tests(
  TARGET runLogParserTests
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# --- Test Executable for Monitoring ---
add_executable(
  runMonitoringTests
  tests/test_monitoring.cpp
  src/LegacyMonitoring.cpp
)

target_include_directories(runMonitoringTests PRIVATE include)

target_link_libraries(
  runMonitoringTests
  gtest_main
)

gtest_add_tests(
  TARGET runMonitoringTests
)

# --- Test Executable for Alerting ---
add_executable(
  runAlertingTests
  tests/test_alerting.cpp
)

target_include_directories(runAlertingTests PRIVATE include)

target_link_libraries(
  runAlertingTests
  gtest_main
)

gtest_add_tests(
  TARGET runAlertingTests
)

# --- Test Executable for CognitiveEngine ---
add_executable(
  runCognitiveEngineTests
  tests/test_cognitive_engine.cpp
  src/CognitiveEngine.cpp
)

target_include_directories(runCognitiveEngineTests PRIVATE include)

target_link_libraries(
  runCognitiveEngineTests
  gtest_main
)

gtest_add_tests(
  TARGET runCognitiveEngineTests
)
