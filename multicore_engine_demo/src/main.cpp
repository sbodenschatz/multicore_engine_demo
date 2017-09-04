/*
 * Multi-Core Engine project
 * File /mutlicore_engine_demo/src/main.cpp
 * Copyright 2015-2017 by Stefan Bodenschatz
 */

#include <boost/filesystem.hpp>
#include <mce/asset/asset_manager.hpp>
#include <mce/asset/load_unit_asset_loader.hpp>
#include <mce/asset/pack_file_reader.hpp>
#include <mce/config/config_store.hpp>
#include <mce/core/engine.hpp>
#include <mce/core/game_state_machine.hpp>
#include <mce/core/window_system.hpp>
#include <mce/demo/test_state.hpp>
#include <mce/demo/version.hpp>
#include <mce/graphics/graphics_system.hpp>
#include <mce/rendering/renderer_system.hpp>

int main(int, char* argv[]) {
	mce::core::engine eng;
	eng.application_metadata({"MCE Demo Application", mce::demo::get_build_version_number()});
	auto loader = std::make_shared<mce::asset::load_unit_asset_loader>(std::vector<mce::asset::path_prefix>(
			{{std::make_unique<mce::asset::pack_file_reader>(), "demoassets.pack"},
			 {std::make_unique<mce::asset::pack_file_reader>(),
			  "../multicore_engine_demo_assets/demoassets.pack"},
			 {std::make_unique<mce::asset::pack_file_reader>(),
			  ((boost::filesystem::path(argv[0]).parent_path().parent_path() /
				"multicore_engine_demo_assets") /
			   "demoassets.pack")
					  .string()}}));
	eng.asset_manager().add_asset_loader(loader);
	eng.asset_manager().start_pin_load_unit("engine/shaders");
	eng.asset_manager().start_pin_load_unit("models_geo");

	// auto res = eng.config_store().resolve<glm::ivec2>("resolution", {800, 600});
	// res->value({1024, 768});
	auto ws = eng.add_system<mce::core::window_system>("Multicore Engine Demo");
	auto gs = eng.add_system<mce::graphics::graphics_system>(*ws);
	eng.add_system<mce::rendering::renderer_system>(*gs);
	eng.game_state_machine().enter<mce::demo::test_state>();

	eng.run();
}
