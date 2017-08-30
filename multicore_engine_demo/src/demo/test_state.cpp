/*
 * Multi-Core Engine project
 * File /multicore_engine_demo/src/demo/test_state.cpp
 * Copyright 2017 by Stefan Bodenschatz
 */

#include <cassert>
#include <mce/demo/test_state.hpp>
#include <mce/rendering/renderer_state.hpp>
#include <mce/rendering/renderer_system.hpp>

namespace mce {
namespace demo {

test_state::test_state(mce::core::engine* engine, mce::core::game_state_machine* state_machine,
					   mce::core::game_state* parent_state)
		: entity_game_state(engine, state_machine, parent_state) {
	auto renderer = engine->get_system<mce::rendering::renderer_system>();
	assert(renderer);
	add_system_state<mce::rendering::renderer_state>(renderer);
}

test_state::~test_state() {}

} /* namespace demo */
} /* namespace mce */
