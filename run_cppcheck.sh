#!/bin/bash
cppcheck -j 8 --std=c++11 -v --inline-suppr --error-exitcode=2 --enable=all -U_MSC_VER -UMULTICORE_ENGINE_WINDOWS -UGLM_DEPTH_CLIP_SPACE -UDEBUG -UMemoryBarrier -I multicore_engine/multicore_engine_base/include/ -I multicore_engine/multicore_engine_core/include/ -I multicore_engine/multicore_engine_renderer/include/ -I multicore_engine_demo/include multicore_engine_demo > /dev/null
