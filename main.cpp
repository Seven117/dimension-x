#include <Geode/Geode.hpp>

using namespace geode::prelude;

// DIMENSION X entry point. Menu hooks live in MainMenu.cpp.
$on_mod(Loaded) {
    log::info("DIMENSION X loaded");
}
