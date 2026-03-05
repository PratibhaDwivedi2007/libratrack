cmake_minimum_required(VERSION 3.16)
project(LibraTrack VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Core library (all modules, no main) ──────────────────────────────────────
file(GLOB_RECURSE LIB_SOURCES "src/*.cpp")
list(FILTER LIB_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

add_library(libratrack_lib STATIC ${LIB_SOURCES})
target_include_directories(libratrack_lib PUBLIC include)

# ── Executable ────────────────────────────────────────────────────────────────
add_executable(libratrack src/main.cpp)
target_link_libraries(libratrack PRIVATE libratrack_lib)

# ── Public tests (always built) ──────────────────────────────────────────────
enable_testing()

find_package(GTest QUIET)
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

file(GLOB PUBLIC_TEST_SOURCES "tests/public/*.cpp")
if(PUBLIC_TEST_SOURCES)
    add_executable(libratrack_tests ${PUBLIC_TEST_SOURCES})
    target_link_libraries(libratrack_tests PRIVATE libratrack_lib GTest::gtest_main)
    target_include_directories(libratrack_tests PRIVATE include)

    include(GoogleTest)
    gtest_discover_tests(libratrack_tests)
else()
    message(STATUS "No public test sources found — skipping libratrack_tests target")
endif()

# ── Private tests (instructor only — provide -DPRIVATE_TEST_DIR=<path>) ───────
if(DEFINED PRIVATE_TEST_DIR)
    file(GLOB PRIVATE_TEST_SOURCES "${PRIVATE_TEST_DIR}/*.cpp")
    if(PRIVATE_TEST_SOURCES)
        add_executable(libratrack_private_tests ${PRIVATE_TEST_SOURCES})
        target_link_libraries(libratrack_private_tests PRIVATE libratrack_lib GTest::gtest_main)
        target_include_directories(libratrack_private_tests PRIVATE include)
        include(GoogleTest)
        gtest_discover_tests(libratrack_private_tests)
        message(STATUS "Private tests enabled from: ${PRIVATE_TEST_DIR}")
    endif()
endif()
