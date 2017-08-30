/*
 * Multi-Core Engine project
 * File /multicore_engine_demo/include/mce/demo/version.hpp
 * Copyright 2017 by Stefan Bodenschatz
 */

#ifndef MCE_DEMO_VERSION_HPP_
#define MCE_DEMO_VERSION_HPP_

#include <mce/core/version.hpp>

namespace mce {
namespace demo {

std::string get_build_version_string();
std::string get_build_version();
std::string get_build_branch();
inline core::version get_build_version_number() {
	return core::parse_version_string(get_build_version());
}

} /* namespace demo */
} /* namespace mce */

#endif /* MCE_DEMO_VERSION_HPP_ */
