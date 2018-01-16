set(TEST_SOURCES
    # Sources here...
    main.cpp)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests Catch ea)

# CTest integration
set(PARSE_CATCH_TESTS_VERBOSE ON)
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(tests)