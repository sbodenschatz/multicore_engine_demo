/*
 * Multi-Core Engine project
 * File /multicore_engine_demo/src/demo/test_state.cpp
 * Copyright 2017 by Stefan Bodenschatz
 */

#include <cassert>
#include <mce/asset/asset_manager.hpp>
#include <mce/config/config_store.hpp>
#include <mce/demo/test_state.hpp>
#include <mce/entity/entity_configuration.hpp>
#include <mce/input/first_person_input_state.hpp>
#include <mce/input/input_system.hpp>
#include <mce/rendering/renderer_state.hpp>
#include <mce/rendering/renderer_system.hpp>
#include <mce/simulation/actuator_state.hpp>

namespace mce {
namespace demo {

test_state::test_state(mce::core::engine* engine, mce::core::game_state_machine* state_machine,
					   mce::core::game_state* parent_state)
		: entity_game_state(engine, state_machine, parent_state) {
	auto rs = add_system_state<mce::rendering::renderer_state>();
	add_system_state<mce::input::first_person_input_state>();
	add_system_state<mce::simulation::actuator_state>();
	auto benchmark_mode = engine->config_store().resolve("demo.benchmark", 0);
	auto ent_asset = engine->asset_manager().load_asset_sync(
			benchmark_mode->value() ? "entities/benchmark.etl" : "entities/demo.etl");
	entity_manager().load_entities_from_template_lang_file(ent_asset);
	if(benchmark_mode->value()) {
		auto objects = engine->config_store().resolve("demo.benchmark.objects", 0)->value();
		auto cube_cfg = entity_manager().find_entity_configuration("cube");
		for(int i = 0; i < objects; ++i) {
			entity_manager().create_entity(cube_cfg);
		}
	}
	rs->camera_preferences()->push_back("main");
}

test_state::~test_state() {}

} /* namespace demo */
} /* namespace mce */
