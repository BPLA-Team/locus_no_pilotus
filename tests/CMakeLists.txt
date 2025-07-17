# language: CMake

# -----------------------------        Subroject setup        -----------------------------
project(tests LANGUAGES CXX)

# set extra C++ compilation flag for plots
set(CMAKE_CXX_FLAGS "-DTEST_PLOTS")

# add all needed files to TEST_SOURCES
file(GLOB_RECURSE TEST_SOURCES "*.h" "*.cpp" "*.ui")

# include dir of all project (for short paths using)
include_directories(${PROJECT_DIR})

# -----------------------------         Add executable         -----------------------------
if(6 GREATER_EQUAL 6)
  qt_add_executable(tests MANUAL_FINALIZATION ${TEST_SOURCES})
else()
  add_executable(tests ${TEST_SOURCES})
endif()

# -----------------------------     Include other libraries    -----------------------------
# Qt libs:
target_link_libraries(tests PUBLIC
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
  Qt6::PrintSupport
)

# Boost libs:
target_link_libraries(tests PRIVATE ${Boost_LIBRARIES})

# QCustomPlot lib:
target_compile_definitions(tests PRIVATE QCUSTOMPLOT_USE_LIBRARY)
target_link_libraries(tests PUBLIC qcustomplot)

# Our lib:
target_link_libraries(tests PRIVATE lib)

# -----------------------------        Set exetra flags        -----------------------------
set_target_properties(tests PROPERTIES
  MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# -----------------------------      Finalize executable       -----------------------------
if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(tests)
endif()
