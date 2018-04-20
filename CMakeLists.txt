cmake_minimum_required (VERSION 3.7)
project (multicore_engine_demo)

include(CMakeConfig.txt)
include(multicore_engine/CMakeUtils.txt)

enable_testing()

if(NOT MSVC)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
endif()

make_build_scripts_project(MCE_DEMO .gitlab-ci.yml .gitlab-ci-edges-env.yml)
get_version_info()

add_subdirectory(multicore_engine)
add_subdirectory(multicore_engine_demo)
if(NOT STATIC_ANALYSIS_ONLY)
	add_subdirectory(multicore_engine_demo_assets)
endif()
