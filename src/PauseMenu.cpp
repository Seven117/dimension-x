#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include "Theme.hpp"

using namespace geode::prelude;
using namespace dimension_x;

// DIMENSION X pause menu layer. Decorates the existing pause screen with the
// same split rails and wordmark as the main menu. Original buttons are untouched.
class $modify(DimensionXPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        if (!Mod::get()->getSettingValue<bool>("themed-pause")) return;
        if (this->getChildByID("dimension-x-pause-rail-green")) return;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // Bottom rails, kept clear of the stock buttons in the middle of the screen.
        if (auto rail = makeGlowStrip(this, { 0.f, 26.f }, { winSize.width * 0.5f, 2.f },
                Palette::green(), 220, 50)) {
            rail->setID("dimension-x-pause-rail-green");
        }
        if (auto rail = makeGlowStrip(this, { winSize.width * 0.5f, 26.f }, { winSize.width * 0.5f, 2.f },
                Palette::cyan(), 220, 50)) {
            rail->setID("dimension-x-pause-rail-cyan");
        }

        if (auto label = CCLabelBMFont::create("DIMENSION X", "goldFont.fnt")) {
            label->setScale(0.32f);
            label->setColor(Palette::mint());
            label->setPosition({ winSize.width / 2.f, 12.f });
            label->setID("dimension-x-pause-title");
            this->addChild(label, 51);
            if (Mod::get()->getSettingValue<bool>("enable-pulse")) {
                label->runAction(makePulse(1.25f, 150, 255));
            }
        }
    }
};
