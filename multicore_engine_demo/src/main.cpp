/*
 * Multi-Core Engine project
 * File /mutlicore_engine_demo/src/main.cpp
 * Copyright 2015-2017 by Stefan Bodenschatz
 */

#include <boost/filesystem.hpp>
#include <iostream>
#include <mce/asset/asset_manager.hpp>
#include <mce/asset/load_unit_asset_loader.hpp>
#include <mce/asset/pack_file_reader.hpp>
#include <mce/config/config_store.hpp>
#include <mce/core/engine.hpp>
#include <mce/core/game_state_machine.hpp>
#include <mce/demo/test_state.hpp>
#include <mce/demo/version.hpp>
#include <mce/entity/parser/entity_text_file_variable_conversion.hpp>
#include <mce/graphics/graphics_system.hpp>
#include <mce/input/input_system.hpp>
#include <mce/rendering/renderer_system.hpp>
#include <mce/simulation/actuator_component.hpp>
#include <mce/simulation/actuator_system.hpp>
#include <mce/windowing/window_system.hpp>
#include <random>

int main(int, char* argv[]) {
	try {
		mce::core::engine eng;
		eng.application_metadata({"MCE Demo Application", mce::demo::get_build_version_number()});
		auto loader =
				std::make_shared<mce::asset::load_unit_asset_loader>(std::vector<mce::asset::path_prefix>(
						{{std::make_unique<mce::asset::pack_file_reader>(), "demo.pack"},
						 {std::make_unique<mce::asset::pack_file_reader>(),
						  "../multicore_engine_demo_assets/demo.pack"},
						 {std::make_unique<mce::asset::pack_file_reader>(),
						  ((boost::filesystem::path(argv[0]).parent_path().parent_path() /
							"multicore_engine_demo_assets") /
						   "demo.pack")
								  .string()},
						 {std::make_unique<mce::asset::pack_file_reader>(), "engine.pack"},
						 {std::make_unique<mce::asset::pack_file_reader>(),
						  "../multicore_engine/multicore_engine_assets/engine.pack"},
						 {std::make_unique<mce::asset::pack_file_reader>(),
						  (((boost::filesystem::path(argv[0]).parent_path().parent_path() /
							 "multicore_engine") /
							"multicore_engine_assets") /
						   "engine.pack")
								  .string()}}));
		eng.asset_manager().add_asset_loader(loader);
		eng.asset_manager().start_pin_load_unit("shaders");
		eng.asset_manager().start_pin_load_unit("models_geo");
		eng.asset_manager().start_pin_load_unit("entities");

		// auto res = eng.config_store().resolve<glm::ivec2>("resolution", {800, 600});
		// res->value({1024, 768});
		auto ws = eng.add_system<mce::windowing::window_system>("Multicore Engine Demo");
		auto gs = eng.add_system<mce::graphics::graphics_system>(*ws);
		auto rs = eng.add_system<mce::rendering::renderer_system>(*gs);
		eng.add_system<mce::input::input_system>(*ws);
		auto as = eng.add_system<mce::simulation::actuator_system>();
		struct random_rotate {
			glm::vec3 angular_velocity;
			random_rotate() {
				std::random_device r;
				std::default_random_engine e(r());
				std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
				do {
					angular_velocity = {dist(e), dist(e), dist(e)};
				} while(dot(angular_velocity, angular_velocity) < 0.0001f);
				angular_velocity = normalize(angular_velocity);
			}
			random_rotate(const random_rotate&) : random_rotate() {}
			void operator()(const mce::core::frame_time& frame_time, mce::entity::entity& ent) {
				glm::quat rot_quat = {0.0f, angular_velocity};
				glm::quat orientation_derivative = 0.5f * (rot_quat * ent.orientation());
				ent.orientation(
						glm::normalize(ent.orientation() + orientation_derivative * frame_time.delta_t));
			}
		};
		as->set_movement_pattern("random_rotate", random_rotate());
		struct orbit {
			bool initialized = false;
			glm::vec3 center;
			float radius = 1.0f;
			float speed = 2.0f;
			float angle = 0.0f;
			glm::vec3 x = {1.0f, 0.0f, 0.0f};
			glm::vec3 y = {0.0f, 1.0f, 0.0f};
			orbit() {
				std::random_device r;
				std::default_random_engine e(r());
				std::uniform_real_distribution<float> dist(0.0f, 360.0f);
				angle = dist(e);
			}
			orbit(const orbit& other)
					: center{other.center}, radius{other.radius}, speed{other.speed}, x{other.x}, y{other.y} {
				std::random_device r;
				std::default_random_engine e(r());
				std::uniform_real_distribution<float> dist(0.0f, 360.0f);
				angle = dist(e);
			}
			void operator()(const mce::core::frame_time& frame_time, mce::entity::entity& ent) {
				if(!initialized) {
					auto ac = ent.component<mce::simulation::actuator_component>();
					if(!ac) return;
					auto it_radius = ac->configuration().unbound_property_values().find("radius");
					if(it_radius != ac->configuration().unbound_property_values().end()) {
						radius = mce::entity::ast::get_variable_value<float>(it_radius->second,
																			 ent.entity_manager());
					}
					auto it_speed = ac->configuration().unbound_property_values().find("speed");
					if(it_speed != ac->configuration().unbound_property_values().end()) {
						speed = mce::entity::ast::get_variable_value<float>(it_speed->second,
																			ent.entity_manager());
					}
					auto it_center = ac->configuration().unbound_property_values().find("center");
					if(it_center != ac->configuration().unbound_property_values().end()) {
						center = mce::entity::ast::get_variable_value<glm::vec3>(it_center->second,
																				 ent.entity_manager());
					} else {
						center = glm::vec3(ent.position().x, ent.position().y, ent.position().z);
					}
					auto it_x = ac->configuration().unbound_property_values().find("x");
					if(it_x != ac->configuration().unbound_property_values().end()) {
						x = mce::entity::ast::get_variable_value<glm::vec3>(it_x->second,
																			ent.entity_manager());
					}
					auto it_y = ac->configuration().unbound_property_values().find("y");
					if(it_y != ac->configuration().unbound_property_values().end()) {
						y = mce::entity::ast::get_variable_value<glm::vec3>(it_y->second,
																			ent.entity_manager());
					}
					initialized = true;
				}
				auto rad = glm::radians(angle);
				glm::vec3 pos = center + radius * (std::sin(rad) * x + std::cos(rad) * y);
				ent.position(glm::vec4(pos, 0.0f));
				angle += speed * frame_time.delta_t;
				while(angle > 360.0f) angle -= 360.0f;
				while(angle < -360.0f) angle += 360.0f;
			}
		};
		as->set_movement_pattern("orbit", orbit());
		rs->material_manager().load_material_library("materials/test");
		eng.game_state_machine().enter<mce::demo::test_state>();

		eng.run();
	} catch(const std::exception& e) {
		std::cerr << e.what();
	}
}
