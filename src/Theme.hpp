#pragma once
#include <Geode/Geode.hpp>

namespace dimension_x {
    using namespace geode::prelude;

    // Palette. ccc3() is used instead of brace-init so these stay valid
    // regardless of how ccColor3B is declared.
    struct Palette {
        static ccColor3B voidBlack() { return ccc3(12, 15, 18); }
        static ccColor3B panel()     { return ccc3(20, 27, 29); }
        static ccColor3B green()     { return ccc3(57, 230, 91); }
        static ccColor3B cyan()      { return ccc3(53, 217, 208); }
        static ccColor3B gold()      { return ccc3(214, 189, 54); }
        static ccColor3B mint()      { return ccc3(169, 230, 176); }
    };

    inline CCLayerColor* makeGlowStrip(
        CCNode* parent,
        CCPoint position,
        CCSize size,
        ccColor3B color,
        uint8_t opacity,
        int z = 0
    ) {
        auto strip = CCLayerColor::create(ccc4(color.r, color.g, color.b, opacity));
        if (!strip) return nullptr;
        strip->setContentSize(size);
        strip->setPosition(position);
        parent->addChild(strip, z);
        return strip;
    }

    // Each node needs its own action instance, so this builds a fresh one per call.
    inline CCRepeatForever* makePulse(float seconds, uint8_t low, uint8_t high) {
        return CCRepeatForever::create(
            CCSequence::create(
                CCFadeTo::create(seconds, low),
                CCFadeTo::create(seconds, high),
                nullptr
            )
        );
    }
}
