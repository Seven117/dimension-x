#pragma once
#include <Geode/Geode.hpp>
#include <algorithm>
#include <cmath>
#include <vector>
#include "Theme.hpp"

// Procedural art for DIMENSION X. Everything here is drawn in code with
// CCDrawNode, so there are no image assets to ship.
namespace dimension_x {
    using namespace geode::prelude;

    constexpr float kPi = 3.14159265f;

    // CCDrawNode blends with premultiplied alpha, so rgb is scaled by alpha.
    inline ccColor4F toF(ccColor3B c, float a) {
        return ccc4f(c.r / 255.f * a, c.g / 255.f * a, c.b / 255.f * a, a);
    }

    inline std::vector<CCPoint> circlePts(CCPoint c, float r, int n = 10) {
        std::vector<CCPoint> pts;
        pts.reserve(n);
        for (int i = 0; i < n; ++i) {
            float a = (static_cast<float>(i) / n) * 2.f * kPi;
            pts.push_back({ c.x + std::cos(a) * r, c.y + std::sin(a) * r });
        }
        return pts;
    }

    // Polygons must be convex: CCDrawNode fills them as a fan.
    inline void fillPoly(CCDrawNode* node, std::vector<CCPoint> pts,
                         ccColor4F fill, ccColor4F border, float borderWidth = 1.f) {
        node->drawPolygon(pts.data(), static_cast<unsigned int>(pts.size()),
                          fill, borderWidth, border);
    }

    // ---------------------------------------------------------------- portal
    inline void drawSpiralArm(CCDrawNode* node, float rotation, float maxRadius,
                              float turns, ccColor4F color, float thickness) {
        constexpr int steps = 48;
        CCPoint prev = { 0.f, 0.f };
        for (int i = 0; i <= steps; ++i) {
            float t = static_cast<float>(i) / steps;
            float r = maxRadius * t;
            float a = rotation + t * turns * 2.f * kPi;
            CCPoint p = { std::cos(a) * r, std::sin(a) * r };
            if (i > 0) node->drawSegment(prev, p, thickness * (0.4f + t), color);
            prev = p;
        }
    }

    // A slowly rotating two-layer vortex with a pulsing core.
    inline CCNode* makePortal(float radius, float alpha) {
        auto root = CCNode::create();
        root->setID("dimension-x-portal");

        auto outer = CCDrawNode::create();
        for (int i = 0; i < 5; ++i) {
            drawSpiralArm(outer, i * 2.f * kPi / 5.f, radius, 1.25f,
                          toF(Palette::cyan(), alpha), radius * 0.012f);
        }
        root->addChild(outer);
        outer->runAction(CCRepeatForever::create(CCRotateBy::create(26.f, 360.f)));

        auto inner = CCDrawNode::create();
        for (int i = 0; i < 4; ++i) {
            drawSpiralArm(inner, i * 2.f * kPi / 4.f, radius * 0.62f, 1.5f,
                          toF(Palette::green(), alpha), radius * 0.010f);
        }
        root->addChild(inner);
        inner->runAction(CCRepeatForever::create(CCRotateBy::create(15.f, -360.f)));

        auto core = CCDrawNode::create();
        core->drawDot({ 0.f, 0.f }, radius * 0.10f, toF(Palette::green(), alpha * 0.9f));
        core->drawDot({ 0.f, 0.f }, radius * 0.045f, toF(Palette::mint(), std::min(1.f, alpha * 1.4f)));
        root->addChild(core);
        core->runAction(CCRepeatForever::create(CCSequence::create(
            CCEaseSineInOut::create(CCScaleTo::create(2.2f, 1.12f)),
            CCEaseSineInOut::create(CCScaleTo::create(2.2f, 0.92f)),
            nullptr
        )));

        return root;
    }

    // ------------------------------------------------------------ characters
    // Two ORIGINAL silhouettes: "Warden", a tall cloaked figure with a glowing
    // visor, and "Sprocket", a hovering one-eyed drone.
    inline CCNode* makeCharacters(float scale) {
        auto root = CCNode::create();
        root->setID("dimension-x-characters");

        auto ink      = toF(ccc3(10, 12, 16), 1.f);
        auto charcoal = toF(Palette::panel(), 1.f);
        auto rimGreen = toF(Palette::green(), 0.55f);
        auto rimCyan  = toF(Palette::cyan(), 0.55f);

        // Warden
        auto warden = CCDrawNode::create();
        fillPoly(warden, { {-24, 0}, {24, 0}, {16, 104}, {-16, 104} }, ink, rimGreen);
        fillPoly(warden, { {-27, 90}, {27, 90}, {20, 108}, {-20, 108} }, charcoal, rimGreen);
        fillPoly(warden, { {-18, 106}, {18, 106}, {23, 128}, {-23, 128} }, charcoal, rimCyan);
        fillPoly(warden, circlePts({ 0.f, 124.f }, 13.f, 10), ink, rimCyan);
        fillPoly(warden, { {-10, 121}, {10, 121}, {9, 128}, {-9, 128} },
                 toF(Palette::cyan(), 1.f), toF(Palette::mint(), 0.8f));
        warden->drawSegment({ -16.f, 52.f }, { 16.f, 52.f }, 1.4f, toF(Palette::green(), 0.8f));
        root->addChild(warden);

        // Sprocket
        auto drone = CCDrawNode::create();
        fillPoly(drone, circlePts({ 52.f, 30.f }, 14.f, 10), ink, rimCyan);
        drone->drawDot({ 52.f, 31.f }, 6.f, toF(Palette::cyan(), 1.f));
        drone->drawDot({ 52.f, 31.f }, 2.8f, toF(ccc3(10, 12, 16), 1.f));
        drone->drawSegment({ 52.f, 44.f }, { 52.f, 56.f }, 0.9f, toF(Palette::green(), 0.8f));
        drone->drawDot({ 52.f, 58.f }, 2.8f, toF(Palette::green(), 1.f));
        root->addChild(drone);
        drone->runAction(CCRepeatForever::create(CCSequence::create(
            CCEaseSineInOut::create(CCMoveBy::create(1.4f, { 0.f, 5.f })),
            CCEaseSineInOut::create(CCMoveBy::create(1.4f, { 0.f, -5.f })),
            nullptr
        )));

        root->setScale(scale);
        return root;
    }
}
