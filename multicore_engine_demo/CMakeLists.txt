cmake_minimum_required (VERSION 3.7)
project(multicore_engine_demo)
file(GLOB_RECURSE DEMO_SRC "src/*.cpp")
file(GLOB_RECURSE DEMO_HEADERS "include/*.hpp")
configure_file(src/demo/version.cpp.in src/demo/version.cpp)
add_executable(mce_demo ${DEMO_SRC} ${DEMO_HEADERS} ${CMAKE_CURRENT_BINARY_DIR}/src/demo/version.cpp src/demo/version.cpp.in)
target_include_directories(mce_demo PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	)
target_link_libraries(mce_demo 
		mce_core
		mce_headed
		mce_renderer
		deps.Boost.ProgramOptions
	)
if(MSVC)
	set_target_properties(mce_demo PROPERTIES LINK_FLAGS_RELEASE "/LTCG")
	set_target_properties(mce_demo PROPERTIES LINK_FLAGS_MINSIZEREL "/LTCG")
	set_target_properties(mce_demo PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/LTCG")
endif()
if(NOT STATIC_ANALYSIS_ONLY)
	add_dependencies(mce_demo mce_demo_assets_packs mce_assets_packs)
endif()
tbb_shared_libs_copy(mce_demo)
