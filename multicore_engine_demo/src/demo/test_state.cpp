/*
 * Multi-Core Engine project
 * File /multicore_engine_demo/src/demo/test_state.cpp
 * Copyright 2017 by Stefan Bodenschatz
 */

#include <cassert>
#include <mce/asset/asset_manager.hpp>
#include <mce/demo/test_state.hpp>
#include <mce/input/input_state.hpp>
#include <mce/input/input_system.hpp>
#include <mce/rendering/renderer_state.hpp>
#include <mce/rendering/renderer_system.hpp>
#include <mce/simulation/actuator_state.hpp>

namespace mce {
namespace demo {

test_state::test_state(mce::core::engine* engine, mce::core::game_state_machine* state_machine,
					   mce::core::game_state* parent_state)
		: entity_game_state(engine, state_machine, parent_state) {
	add_system_state<mce::rendering::renderer_state>();
	add_system_state<mce::input::input_state>();
	add_system_state<mce::simulation::actuator_state>();
	auto ent_asset = engine->asset_manager().load_asset_sync("entities/demo.etf");
	entity_manager().load_entities_from_template_lang_file(ent_asset);
}

test_state::~test_state() {}

} /* namespace demo */
} /* namespace mce */
