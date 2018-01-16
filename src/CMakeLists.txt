set(SOURCE_FILES
    # Source files here...
    dll.h
    main.cpp
    functions.h
    )

# Create the executable
add_executable(table_seating_ea ${SOURCE_FILES})

add_definitions(-DBUILDING_DLL)

# Create the library
add_library(ea SHARED ${SOURCE_FILES})

# Link required libraries
target_link_libraries(table_seating_ea ${LIBRARIES})

# Mark this directory as a root directory for future includes
target_include_directories(ea PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})