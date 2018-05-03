cmake_minimum_required (VERSION 3.10)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_helpers")

include(RepositoryVersionDetector)
get_version_info()

project (multicore_engine_demo
		VERSION ${BUILD_VERSION_MAJOR}.${BUILD_VERSION_MINOR}.${BUILD_VERSION_PATCH}.${BUILD_VERSION_TWEAK} 
		# Language C is needed for pthreads detection to work correctly
		LANGUAGES CXX C
	)

include(IntegrateBuildScripts)
make_build_scripts_project(MCE_DEMO .gitlab-ci.yml .gitlab-ci-edges-env.yml)

include(BuildConfig.cmake)

option(MCEDEMO_MCE_AS_SUBDIRECTORY "Use mce as an embedded subdirectory (uses find_package otherwise)." ON)

if(MCEDEMO_MCE_AS_SUBDIRECTORY)
	include("${CMAKE_CURRENT_LIST_DIR}/multicore_engine/UseModules.cmake")
	add_subdirectory(multicore_engine)
else()
	find_package(mce REQUIRED)
	include("${mce_DIR}/UseModules.cmake")
endif()

enable_testing()

find_package(TBB REQUIRED)
add_subdirectory(multicore_engine_demo)
if(NOT STATIC_ANALYSIS_ONLY)
	add_subdirectory(multicore_engine_demo_assets)
endif()

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT mce_demo)

install(
		FILES ${CMAKE_CURRENT_SOURCE_DIR}/multicore_engine/default.cfg
		DESTINATION .
		COMPONENT mce-demo
	)

include(ComponentInstallTargets)
set(INSTALL_COMPONENT_mce-demo_DEPENDS mce_demo)
define_component_install_targets(mce-demo)
