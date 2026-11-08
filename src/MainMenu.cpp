#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <algorithm>
#include <filesystem>
#include "Theme.hpp"
#include "Art.hpp"

using namespace geode::prelude;
using namespace dimension_x;

// DIMENSION X visual layer. It decorates the existing menu rather than deleting
// original buttons, so navigation stays usable while the redesign grows.
class $modify(DimensionXMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        // Guard against the decoration being added twice.
        if (this->getChildByID("dimension-x-footer")) return true;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto mod = Mod::get();

        bool pulseEnabled = mod->getSettingValue<bool>("enable-pulse");
        auto footerOpacity = static_cast<uint8_t>(
            std::clamp<int64_t>(mod->getSettingValue<int64_t>("footer-opacity"), 0, 255)
        );

        // ---- Background layers (custom image, portal) ----
        // These are attached to the stock background node itself, so they draw
        // with it, underneath the menu buttons.
        if (auto bg = this->getChildByID("main-menu-bg")) {
            auto bgSize = bg->getContentSize();
            CCPoint bgCenter = { bgSize.width / 2.f, bgSize.height / 2.f };

            auto imagePath = mod->getSettingValue<std::filesystem::path>("custom-background");
            std::error_code ec;
            if (!imagePath.empty() && std::filesystem::exists(imagePath, ec)) {
                if (auto img = CCSprite::create(imagePath.string().c_str())) {
                    auto sz = img->getContentSize();
                    if (sz.width > 0.f && sz.height > 0.f) {
                        img->setScale(std::max(bgSize.width / sz.width, bgSize.height / sz.height));
                        img->setPosition(bgCenter);
                        img->setID("dimension-x-custom-bg");
                        bg->addChild(img, 1);
                    }
                }
            }

            if (mod->getSettingValue<bool>("portal-background")) {
                float alpha = std::clamp<int64_t>(mod->getSettingValue<int64_t>("portal-opacity"), 0, 255) / 255.f;
                auto portal = makePortal(std::min(bgSize.width, bgSize.height) * 0.42f, alpha);
                portal->setPosition(bgCenter);
                bg->addChild(portal, 2);
            }
        } else {
            log::warn("DIMENSION X: node 'main-menu-bg' not found; skipping background layers");
        }

        if (mod->getSettingValue<bool>("show-characters")) {
            auto chars = makeCharacters(0.9f);
            chars->setPosition({ winSize.width * 0.14f, 46.f });
            this->addChild(chars, 5);
        }

        // Dark translucent footer band anchors the custom wordmark.
        auto black = Palette::voidBlack();
        auto footer = CCLayerColor::create(ccc4(black.r, black.g, black.b, footerOpacity));
        if (footer) {
            footer->setContentSize({ winSize.width, 42.f });
            footer->setPosition({ 0.f, 0.f });
            footer->setID("dimension-x-footer");
            this->addChild(footer, 900);
        }

        // Thin portal-green and cyan rails create the dual-dimension motif.
        if (auto rail = makeGlowStrip(this, { 0.f, 40.f }, { winSize.width * 0.5f, 2.f },
                Palette::green(), 230, 901)) {
            rail->setID("dimension-x-rail-green");
        }
        if (auto rail = makeGlowStrip(this, { winSize.width * 0.5f, 40.f }, { winSize.width * 0.5f, 2.f },
                Palette::cyan(), 230, 901)) {
            rail->setID("dimension-x-rail-cyan");
        }

        auto title = CCLabelBMFont::create("DIMENSION X", "goldFont.fnt");
        if (title) {
            title->setScale(0.48f);
            title->setColor(Palette::mint());
            title->setPosition({ winSize.width / 2.f, 24.f });
            title->setID("dimension-x-title");
            this->addChild(title, 902);
            if (pulseEnabled) title->runAction(makePulse(1.25f, 150, 255));
        }

        auto subtitle = CCLabelBMFont::create("PORTAL FLUID  //  EVIL MORTY", "goldFont.fnt");
        if (subtitle) {
            subtitle->setScale(0.20f);
            subtitle->setColor(Palette::gold());
            subtitle->setOpacity(220);
            subtitle->setPosition({ winSize.width / 2.f, 9.f });
            subtitle->setID("dimension-x-subtitle");
            this->addChild(subtitle, 902);
            if (pulseEnabled) subtitle->runAction(makePulse(1.25f, 150, 255));
        }

        log::info("DIMENSION X main menu visual layer initialized");
        return true;
    }
};
