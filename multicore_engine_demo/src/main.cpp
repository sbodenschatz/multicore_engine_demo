/*
 * Multi-Core Engine project
 * File /mutlicore_engine_demo/src/main.cpp
 * Copyright 2015 by Stefan Bodenschatz
 */

#include <mce/core/engine.hpp>
#include <mce/core/window_system.hpp>
#include <mce/graphics/graphics_system.hpp>
#include <mce/rendering/renderer_system.hpp>

int main() {
	mce::core::engine eng;
	auto ws = eng.add_system<mce::core::window_system>(9, 9);
	auto gs = eng.add_system<mce::graphics::graphics_system>(10, 10, *ws);
	eng.add_system<mce::rendering::renderer_system>(11, 11, *gs);
}
