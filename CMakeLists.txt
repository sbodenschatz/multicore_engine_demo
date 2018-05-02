cmake_minimum_required (VERSION 3.10)
project (multicore_engine_demo)

include("${CMAKE_CURRENT_LIST_DIR}/multicore_engine/UseModules.cmake")

include(BuildConfig.cmake)

enable_testing()

include(IntegrateBuildScripts)
make_build_scripts_project(MCE_DEMO .gitlab-ci.yml .gitlab-ci-edges-env.yml)
include(RepositoryVersionDetector)
get_version_info()

add_subdirectory(multicore_engine)
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
