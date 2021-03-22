cmake_minimum_required(VERSION 3.13)

project(objdump-parser CXX)

find_package(Threads)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(FILAMENT_REQUIRES_CXXABI ON)

set(TARGET_PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS ON)

include(build/conanbuildinfo.cmake)
conan_basic_setup()

if(CONAN_CATCH2_ROOT_DEBUG)
  include(${CONAN_CATCH2_ROOT_DEBUG}/lib/cmake/Catch2/Catch.cmake)
else()
  include(${CONAN_CATCH2_ROOT}/lib/cmake/Catch2/Catch.cmake)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(COMPILATION_COMMANDS
    -fcoroutines
    -fconcepts
    -O0
    # -O3
    -g
    -Wall
    # -Weffc++
    -pedantic
    -pedantic-errors
    -Wextra
    -Wcast-align
    -Wcast-qual
    -Wchar-subscripts
    -Wcomment
    # -Wconversion
    # -Wdisabled-optimization
    -Wfloat-equal
    -Wformat
    -Wformat=2
    -Wformat-nonliteral
    -Wformat-security
    -Wformat-y2k
    -Wimport
    -Winit-self
    # -Winline
    -Winvalid-pch
    # -Wmissing-braces
    -Wmissing-field-initializers
    -Wmissing-format-attribute
    -Wmissing-include-dirs
    -Wmissing-noreturn
    -Wpacked
    -Wparentheses
    -Wpointer-arith
    -Wredundant-decls
    -Wreturn-type
    -Wsequence-point
    # -Wshadow
    -Wsign-compare
    # -Wstack-protector
    -Wstrict-aliasing
    -Wstrict-aliasing=2
    -Wswitch
    # -Wswitch-default
    -Wswitch-enum
    -Wtrigraphs
    -Wuninitialized
    -Wunknown-pragmas
    -Wunreachable-code
    # -Wunused-function
    # -Wunused-label
    # -Wunused-parameter
    # -Wunused-value
    -Wno-unused-variable
    -Wno-switch-enum
    -Wvariadic-macros
    -Wvolatile-register-var
    -Wwrite-strings
    -Wsign-compare
    # -Wsign-conversion
    -Wfloat-conversion
    -Wpedantic
    # -march=native
    # -flto
    # -ftree-vectorize
    # -msse4
    # -mfpmath=sse
    )
endif()

add_executable(objdump-parser
  # ${CMAKE_SOURCE_DIR}/src/input.cpp
  # ${CMAKE_SOURCE_DIR}/src/create_log_tree.cpp
  ${CMAKE_SOURCE_DIR}/src/main.cpp)

target_include_directories(objdump-parser PRIVATE
  inc)

target_compile_options(objdump-parser PRIVATE ${COMPILATION_COMMANDS})
set_target_properties(objdump-parser PROPERTIES ${TARGET_PROPERTIES})
target_link_libraries(objdump-parser ${CONAN_LIBS})
target_precompile_headers(objdump-parser PUBLIC
  inc/std.hpp
  inc/boost.hpp
  inc/ranges.hpp
  inc/fmt.hpp)

add_executable(objdump-parser_tests
  tests/test_main.cpp
  # ${CMAKE_SOURCE_DIR}/src/create_log_tree.cpp
  # ${CMAKE_SOURCE_DIR}/src/input.cpp
  # ${CMAKE_SOURCE_DIR}/tests/test_command_line_options.cpp
  # ${CMAKE_SOURCE_DIR}/tests/test_create_log_tree.cpp
  # ${CMAKE_SOURCE_DIR}/tests/test_date.cpp
  # ${CMAKE_SOURCE_DIR}/tests/test_log_line_info.cpp
  # ${CMAKE_SOURCE_DIR}/tests/test_pretty_print_json.cpp
  # ${CMAKE_SOURCE_DIR}/tests/test_read_log_file.cpp
  # ${CMAKE_SOURCE_DIR}/tests/test_split_log_string.cpp
  # ${CMAKE_SOURCE_DIR}/tests/test_trace_tags.cpp
  )

add_custom_target(create_objdump
  # COMMAND "objdump -S -s -l -d --demangle build/bin/objdump-parser > build/objdump-parser.objdump")
  COMMAND objdump -S -s -l -d --demangle ${CMAKE_BINARY_DIR}/bin/objdump-parser > ${CMAKE_BINARY_DIR}/objdump-parser.objdump
  BYPRODUCTS ${CMAKE_BINARY_DIR}/objdump-parser.objdump)


target_precompile_headers(objdump-parser_tests REUSE_FROM objdump-parser)
# target_precompile_headers(objdump-parser_tests
#   PUBLIC
#   # ${CONAN_INCLUDE_DIRS_CATCH2}/catch2/catch.hpp
#   )

target_include_directories(objdump-parser_tests PRIVATE inc)
target_compile_options(objdump-parser_tests PRIVATE ${COMPILATION_COMMANDS})
target_compile_definitions(objdump-parser_tests PRIVATE CATCH_CONFIG_ENABLE_BENCHMARKING CATCH_CONFIG_FAST_COMPILE CATCH_CONFIG_USE_ASYNC)
set_target_properties(objdump-parser_tests PROPERTIES ${TARGET_PROPERTIES})
target_link_libraries(objdump-parser_tests ${CONAN_LIBS})

enable_testing()

catch_discover_tests(objdump-parser_tests
  EXTRA_ARGS -s --reporter=xml --out=constexpr.xml)
