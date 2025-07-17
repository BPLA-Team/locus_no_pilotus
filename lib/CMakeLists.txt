# language: CMake

# ---------------------------------    Subroject setup    ---------------------------------
project(lib LANGUAGES CXX)

# add all needed files to LIB_SOURCES
file(GLOB_RECURSE LIB_SOURCES "*.h" "*.cpp"
  "${MATH_DIR}/*.h" "${MATH_DIR}/*.cpp")

# include dir of all project (for short paths using)
include_directories(${PROJECT_DIR})

# make it static library (to use in other subprojects)
add_library(lib STATIC ${LIB_SOURCES})

# -----------------------------     Include other libraries    -----------------------------
# Qt libs:
target_link_libraries(lib PRIVATE Qt6::Core)
