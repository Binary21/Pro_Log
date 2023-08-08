cmake_minimum_required(VERSION 3.5)
project(VTprolog CXX)

# You will need to edit these variables as the project progresses
#-----------------------------------------------------------------------

# add source for library modules here
set(LIB_SOURCE
  Token.hpp 
  Token.cpp 
  Lexer.cpp 
  Lexer.hpp 
  ExpressionTree.cpp 
  ExpressionTree.hpp
  Parser.cpp
  Parser.hpp
  KnowledgeBase.cpp
  KnowledgeBase.hpp
  Unification.cpp
  Unification.hpp
  KnowledgeBaseUtility.cpp
  KnowledgeBaseUtility.hpp

)

# add source for library unit tests here
set(LIB_TEST_SOURCE
  UnitTests.cpp
  TokenTests.cpp
  LexerTests.cpp
  ExpressionTreeTests.cpp
  ParserTests.cpp
  KnowledgeBaseTests.cpp
  UnificationTests.cpp
  KnowledgeBaseUtilityTests.cpp
)

# add source for tui modules here
set(TUI_SOURCE
  )

# add source for tui functional tests here
set(TUI_TEST_SOURCE
  TUITests.cpp
  )

# add source for gui modules here
set(GUI_SOURCE
  Gui.hpp
  Gui.cpp
  )

# add source for gui functional tests here
set(GUI_TEST_SOURCE
  GUITests.cpp
  )

# add source for vtpl program here
set(VTPL_SOURCE
  vtpl.cpp
  )

# add source for vtpl-ide program here
set(VTPLIDE_SOURCE
  vtpl-ide.cpp
  )

# You should not need to edit below this line
#-----------------------------------------------------------------------
#-----------------------------------------------------------------------

# export compilation database for tool use
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# try to prevent accidental in-source builds, these cause lots of problems
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds not allowed. Remove any files created thus far and use a different directory for the build.")
endif()

# require a C++11 compiler for all targets
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# optional strict mode in reference environment
if(UNIX AND CMAKE_COMPILER_IS_GNUCXX AND STRICT)
  message("-- Enabling strict compilation mode")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

# setup testing config file
set(TEST_FILE_DIR "${CMAKE_SOURCE_DIR}/tests")
configure_file(${CMAKE_SOURCE_DIR}/test_config.hpp.in 
  ${CMAKE_BINARY_DIR}/test_config.hpp)
include_directories(${CMAKE_BINARY_DIR})

# setup catch
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(Catch)

# configure Qt
find_package(Qt5 COMPONENTS Core Widgets Test QUIET)
if(Qt5_FOUND)
  message("-- Enabling Qt")
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
else(Qt5_FOUND)
  message("-- Warning: Qt not found.")
endif()

# executables without Qt dependency
add_executable(unit_tests catch.hpp ${LIB_TEST_SOURCE} ${LIB_SOURCE})
if(UNIX AND NOT APPLE AND CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(unit_tests pthread)
endif()

add_executable(tui_test ${TUI_TEST_SOURCE} ${TUI_SOURCE} ${LIB_SOURCE})
if(UNIX AND NOT APPLE AND CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(tui_test pthread)
endif()

add_executable(vtpl ${VTPL_SOURCE} ${TUI_SOURCE} ${LIB_SOURCE})
if(UNIX AND NOT APPLE AND CMAKE_COMPILER_IS_GNUCXX)
  target_link_libraries(vtpl pthread)
endif()


# executables with Qt dependency
if(Qt5_FOUND)

  add_executable(gui_test ${GUI_TEST_SOURCE} ${GUI_SOURCE} ${LIB_SOURCE})
  if(UNIX AND NOT APPLE AND CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(gui_test Qt5::Widgets Qt5::Test pthread)
  else(UNIX AND NOT APPLE AND CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(gui_test Qt5::Widgets Qt5::Test)
  endif()
  
  add_executable(vtpl-ide ${VTPLIDE_SOURCE} ${GUI_SOURCE} ${LIB_SOURCE})
  if(UNIX AND NOT APPLE AND CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(vtpl-ide Qt5::Widgets pthread)
  else(UNIX AND NOT APPLE AND CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(vtpl-ide Qt5::Widgets)
  endif()


endif() # end Qt dependency

# Testing
include(CTest)

# register Catch tests with cmake
catch_discover_tests(unit_tests)

# In the reference environment enable coverage on tests and threading
if(UNIX AND NOT APPLE AND CMAKE_COMPILER_IS_GNUCXX AND COVERAGE)
  message("-- Enabling vtpl library test coverage")
  set(GCC_COVERAGE_COMPILE_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
  set_target_properties(unit_tests PROPERTIES COMPILE_FLAGS ${GCC_COVERAGE_COMPILE_FLAGS} )
  target_link_libraries(unit_tests gcov)
  add_custom_target(coverage
    COMMAND ${CMAKE_COMMAND} -E env "ROOT=${CMAKE_CURRENT_SOURCE_DIR}"
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/coverage.sh)
endif()

# In the reference environment enable memory checking on tests
if(UNIX AND NOT APPLE AND CMAKE_COMPILER_IS_GNUCXX AND MEMTEST)
  message("-- Enabling vtpl library memory checks")
  add_custom_target(memtest
    COMMAND valgrind ${CMAKE_BINARY_DIR}/unit_tests)
endif()
