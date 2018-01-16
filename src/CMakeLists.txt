set(SOURCE_FILES
    # Source files here...
    dll.h
    main.cpp
    functions.h
    )

# Create the executable
add_executable(table_seating_ga ${SOURCE_FILES})

add_definitions(-DBUILDING_DLL)

# Create the library
add_library(ga SHARED ${SOURCE_FILES})

# Link required libraries
target_link_libraries(table_seating_ga ${LIBRARIES})

# Mark this directory as a root directory for future includes
target_include_directories(ga PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})