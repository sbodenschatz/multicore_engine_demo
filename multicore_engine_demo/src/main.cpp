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
#include <mce/entity/parser/entity_template_lang_variable_conversion.hpp>
#include <mce/graphics/graphics_system.hpp>
#include <mce/input/input_system.hpp>
#include <mce/rendering/renderer_system.hpp>
#include <mce/simulation/actuator_component.hpp>
#include <mce/simulation/actuator_system.hpp>
#include <mce/util/statistics.hpp>
#include <mce/windowing/window_system.hpp>
#include <random>

class random {
	std::mutex mtx;
	std::random_device r;
	std::default_random_engine e;
	std::uniform_real_distribution<float> dist_vec;
	std::uniform_real_distribution<float> dist_angle;

public:
	random() : e(r()), dist_vec(-1.0f, 1.0f), dist_angle(0.0f, 360.0f) {}
	float random_vec_comp() {
		std::lock_guard<std::mutex> lock(mtx);
		return dist_vec(r);
	}
	float random_angle() {
		std::lock_guard<std::mutex> lock(mtx);
		return dist_angle(r);
	}
};

static class random rnd;

int main(int, char* argv[]) {
	try {
		mce::core::engine eng;
		eng.application_metadata({"MCE Demo Application", mce::demo::get_build_version_number()});
		auto loader =
				std::make_shared<mce::asset::load_unit_asset_loader>(std::vector<mce::asset::path_prefix>(
						{{std::make_unique<mce::asset::pack_file_reader>(), "demo.pack"},
						 {std::make_unique<mce::asset::pack_file_reader>(), "../demo.pack"},
						 {std::make_unique<mce::asset::pack_file_reader>(),
						  "../multicore_engine_demo_assets/demo.pack"},
						 {std::make_unique<mce::asset::pack_file_reader>(),
						  ((boost::filesystem::path(argv[0]).parent_path().parent_path() /
							"multicore_engine_demo_assets") /
						   "demo.pack")
								  .string()},
						 {std::make_unique<mce::asset::pack_file_reader>(), "engine.pack"},
						 {std::make_unique<mce::asset::pack_file_reader>(), "../engine.pack"},
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
		eng.asset_manager().start_pin_load_unit("materials");

		// auto res = eng.config_store().resolve<glm::ivec2>("window.resolution", {800, 600});
		// res->value({1024, 768});
		auto ws = eng.add_system<mce::windowing::window_system>("Multicore Engine Demo");
		auto gs = eng.add_system<mce::graphics::graphics_system>(*ws);
		auto rs = eng.add_system<mce::rendering::renderer_system>(*gs);
		eng.add_system<mce::input::input_system>(*ws);
		auto as = eng.add_system<mce::simulation::actuator_system>();
		struct random_rotate {
			glm::vec3 angular_velocity;
			random_rotate() {
				do {
					angular_velocity = {rnd.random_vec_comp(), rnd.random_vec_comp(), rnd.random_vec_comp()};
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
				angle = rnd.random_angle();
			}
			orbit(const orbit& other)
					: center{other.center}, radius{other.radius}, speed{other.speed}, x{other.x}, y{other.y} {
				angle = rnd.random_angle();
				speed = rnd.random_vec_comp() * 20.0f;
				radius = std::abs(rnd.random_vec_comp()) * 20.0f;
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
		rs->material_manager().load_material_library("materials/demo");
		size_t objects = 0;
		eng.game_state_machine().enter<mce::demo::test_state>(objects);
		auto benchmark_mode = eng.config_store().resolve("demo.benchmark", 0);
		if(benchmark_mode->value()) {
			objects -= 2;
			auto write_stats = [&eng, objects]() {
				auto aggregate_ft =
						eng.statistics_manager()
								.get<mce::util::aggregate_statistic<std::chrono::microseconds::rep>>(
										"core.frametime.aggregate");
				aggregate_ft->labels()->header[0] = "objects";
				aggregate_ft->labels()->prefix = std::to_string(objects);
				aggregate_ft->append_output(true);
				auto hist_ft = eng.statistics_manager()
									   .get<mce::util::histogram_statistic<std::chrono::microseconds::rep>>(
											   "core.frametime.histogram");
				hist_ft->labels()->header[0] = "objects";
				hist_ft->labels()->prefix = std::to_string(objects);
				eng.statistics_manager().save("\t");
			};
			ws->window().key_callback([&eng, write_stats, objects](mce::glfw::key key, int,
																   mce::glfw::button_action button_action,
																   mce::glfw::modifier_flags) {
				if(button_action != mce::glfw::button_action::press) return;
				if(key == mce::glfw::key::k_f11) {
					eng.statistics_manager().clear_values();
				} else if(key == mce::glfw::key::k_f10) {
					write_stats();
				} else if(key == mce::glfw::key::k_f9) {
					eng.statistics_manager().clear_values();
					std::thread t([&eng, write_stats, objects]() {
						using namespace std::chrono_literals;
						std::this_thread::sleep_for(1min);
						write_stats();
						eng.stop();
						boost::filesystem::rename("stats/core.frametime.histogram.csv",
												  "stats/" + std::to_string(objects) +
														  ".frametime.histogram.csv");
					});
					t.detach();
				}
			});
		}

		eng.run();
	} catch(const std::exception& e) {
		std::cerr << e.what();
	}
}
