# language: CMake

# -----------------------------        Subroject setup        -----------------------------
project(main LANGUAGES CXX)

# add all needed files to PROJECT_SOURCES
file(GLOB_RECURSE PROJECT_SOURCES "*.h" "*.cpp" "*.ui"
  "${GUI_DIR}/*.h" "${GUI_DIR}/*.cpp" "${DATATOOLS_DIR}/*.h" "${DATATOOLS_DIR}/*.cpp")

# include dir of all project (for short paths using)
include_directories(${PROJECT_DIR})

# -----------------------------         Add executable         -----------------------------
if(6 GREATER_EQUAL 6)
  qt_add_executable(main MANUAL_FINALIZATION ${PROJECT_SOURCES})
else()
  add_executable(main ${PROJECT_SOURCES})
endif()

# ------------------------------    Include other libraries    -----------------------------
# Qt libs:
target_link_libraries(main PRIVATE
  Qt6::Widgets
  Qt6::Core
)

# QCustomPlot lib:
target_compile_definitions(main PRIVATE QCUSTOMPLOT_USE_LIBRARY)
target_link_libraries(main PUBLIC qcustomplot)

# Our lib:
target_link_libraries(main PRIVATE lib)

# -----------------------------        Set exetra flags        -----------------------------
set_target_properties(main PROPERTIES
  MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# -----------------------------      Finalize executable       -----------------------------
if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(main)
endif()
