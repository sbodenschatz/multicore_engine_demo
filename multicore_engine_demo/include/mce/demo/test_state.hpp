/*
 * Multi-Core Engine project
 * File /multicore_engine_demo/include/mce/demo/test_state.hpp
 * Copyright 2017 by Stefan Bodenschatz
 */

#ifndef MCE_DEMO_TEST_STATE_HPP_
#define MCE_DEMO_TEST_STATE_HPP_

#include <mce/core/entity_game_state.hpp>

namespace mce {
namespace demo {

class test_state : public core::entity_game_state {
public:
	test_state(mce::core::engine* engine, mce::core::game_state_machine* state_machine,
			   mce::core::game_state* parent_state, size_t& object_count);
	virtual ~test_state();
};

} /* namespace demo */
} /* namespace mce */

#endif /* MCE_DEMO_TEST_STATE_HPP_ */
