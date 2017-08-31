/*
 * Multi-Core Engine project
 * File /mutlicore_engine_demo/src/main.cpp
 * Copyright 2015-2017 by Stefan Bodenschatz
 */

#include <mce/config/config_store.hpp>
#include <mce/core/engine.hpp>
#include <mce/core/game_state_machine.hpp>
#include <mce/core/window_system.hpp>
#include <mce/demo/test_state.hpp>
#include <mce/demo/version.hpp>
#include <mce/graphics/graphics_system.hpp>
#include <mce/rendering/renderer_system.hpp>

int main() {
	mce::core::engine eng;
	eng.application_metadata({"MCE Demo Application", mce::demo::get_build_version_number()});
	// auto res = eng.config_store().resolve<glm::ivec2>("resolution", {800, 600});
	// res->value({1024, 768});
	auto ws = eng.add_system<mce::core::window_system>("Multicore Engine Demo");
	auto gs = eng.add_system<mce::graphics::graphics_system>(*ws);
	eng.add_system<mce::rendering::renderer_system>(*gs);
	eng.game_state_machine().enter<mce::demo::test_state>();

	eng.run();
}
