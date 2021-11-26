cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(path_planningStandalone LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(NAME path_planning SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/path_planning/*.cpp)

add_executable(path_planningStandalone ${sources})

set_target_properties(path_planningStandalone PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "path_planning_example")

target_link_libraries(path_planningStandalone path_planning)


# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/clipper/*.cpp)

add_executable(clipper_canStandalone ${sources})

set_target_properties(clipper_canStandalone PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "clipper_example")

target_link_libraries(clipper_canStandalone path_planning)