find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(GraphTests test_main.cpp GraphTest.cpp WideBridgesTests.cpp)
target_link_libraries(GraphTests ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(boostUnitTestsRun GraphTests)

if (CMAKE_CONFIGURATION_TYPES)
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
      --force-new-ctest-process --output-on-failure
      --build-config "$<CONFIGURATION>"
      DEPENDS GraphTests)
else()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
      --force-new-ctest-process --output-on-failure
      DEPENDS GraphTests)
endif()
