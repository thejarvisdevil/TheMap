#include <Geode/Geode.hpp>
using namespace geode::prelude;

#include <Geode/cocos/CCDirector.h>
#include <Geode/binding/GameLevelManager.hpp>
#include <Geode/binding/LevelInfoLayer.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/utils/web.hpp>
#include "levels.hpp"

#include <filesystem>
#include <string>

// i should really implement the thing where the actual bgm plays when exiting a map level

static bool c_custom = false;
static std::vector<std::string> c_customBG;
static std::vector<std::string> g_customBGM;

static std::vector<World> customTM(float width, float height) {
    auto configDir = Mod::get()->getConfigDir(true);
    std::string customDir = (configDir / "custom").string();
    geode::utils::file::createDirectory(customDir);

    auto gg = geode::utils::file::readString(customDir + "/map.json");
    if (!gg) {
        c_custom = false;
        c_customBG.clear();
        g_customBGM.clear();
        return getWorlds(width, height);
    }

    auto yayyyy = matjson::parse(*gg); // matpat parse
    if (!yayyyy) {
        c_custom = false;
        c_customBG.clear();
        g_customBGM.clear();
        return getWorlds(width, height);
    }

    std::vector<World> worlds;
    c_customBG.clear();
    g_customBGM.clear();

    for (auto& wv : *yayyyy) {
        World w;
        w.title = wv["title"].asString().unwrapOr("");
        w.desc  = wv["shortText"].asString().unwrapOr("");
        std::string bg = wv["background"].asString().unwrapOr("");
        std::string music = wv["music"].asString().unwrapOr("");
        w.audio = "";

        if (wv["levels"].isArray()) {
            for (auto& lv : wv["levels"]) {
                WorldLevel l;
                double rawx = lv["x"].asDouble().unwrapOr(0.0);
                double rawy = lv["y"].asDouble().unwrapOr(0.0);
                l.x = (rawx > 0 && rawx <= 1) ? float(rawx * width) : float(rawx);
                l.y = (rawy > 0 && rawy <= 1) ? float(rawy * height) : float(rawy);
                l.levelID = lv["levelID"].asInt().unwrapOr(0);
                l.order = lv["order"].asInt().unwrapOr(0);
                l.authorName = lv["authorName"].asString().unwrapOr("");
                l.accountID = lv["accountID"].asInt().unwrapOr(0);
                l.userID = lv["userID"].asInt().unwrapOr(0);
                w.levels.push_back(l);
            }
        }

        worlds.push_back(w);
        c_customBG.push_back(bg);
        g_customBGM.push_back(music);
    }

    if (worlds.empty()) {
        c_custom = false;
        return getWorlds(width, height);
    }

    c_custom = true;
    return worlds;
}

class WorldLvl : public LevelDownloadDelegate {
public:
    WorldLevel wlvl;
    void levelDownloadFinished(GJGameLevel* level) override {
        GameLevelManager::get()->m_levelDownloadDelegate = nullptr;
        level->m_creatorName = wlvl.authorName;
        level->m_accountID = wlvl.accountID;
        level->m_userID = wlvl.userID;
        auto scene = LevelInfoLayer::scene(level, false);
        scene->setUserObject("is-world-level"_spr, CCBool::create(true));
        CCDirector::get()->pushScene(CCTransitionFade::create(0.5f, scene));
    }
    void levelDownloadFailed(int id) override {
        FLAlertLayer::create("Download Failed",
            "Failed to download the level. Please try again later.",
            "OK")->show();
        GameLevelManager::get()->m_levelDownloadDelegate = nullptr;
    }
};
static inline WorldLvl s_delegate;

class WorldsLayer : public cocos2d::CCLayer {
public:
    CREATE_FUNC(WorldsLayer);

    size_t theWorldWeAreIn = 0;
    CCMenuItemSpriteExtra* prevBtn = nullptr;
    CCMenuItemSpriteExtra* nextBtn = nullptr;
    std::string bgmAudio;

    bool init() override {
        if (!CCLayer::init()) return false;
        auto win = CCDirector::get()->getWinSize();

        auto menu = CCMenu::create();
        menu->setID("world-menu");
        menu->setPosition({0,0});
        addChild(menu);

        auto discordSpr = CCSprite::createWithSpriteFrameName("gj_discordIcon_001.png");
        auto discordBtn = CCMenuItemSpriteExtra::create(discordSpr, nullptr, this, menu_selector(WorldsLayer::onDiscord));
        discordBtn->setID("discord-btn");
        discordBtn->setPosition({
            win.width - discordSpr->getContentSize().width/2 - 13.f,
            discordSpr->getContentSize().height/2 + 20.f
        });
        menu->addChild(discordBtn);

        auto title = CCLabelBMFont::create("", "bigFont.fnt");
        title->setID("world-name");
        title->setPosition(win.width / 2, win.height - 25.f);
        title->setScale(0.6f);
        this->addChild(title);

        auto funnytext = CCLabelBMFont::create("", "bigFont.fnt");
        funnytext->setID("funny-text");
        funnytext->setPosition(win.width / 2, win.height / 1.15f);
        funnytext->setScale(0.4f);
        this->addChild(funnytext);

        auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        auto backBtn = CCMenuItemSpriteExtra::create(backSpr, nullptr, this, menu_selector(WorldsLayer::onBack));
        backBtn->setPosition({30.f, win.height - 30.f});
        backBtn->setID("back-btn");
        menu->addChild(backBtn);

        auto prevSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        prevBtn = CCMenuItemSpriteExtra::create(prevSpr, nullptr, this, menu_selector(WorldsLayer::onMINUSWORLD));
        prevBtn->setPosition({50.f, win.height / 2});
        prevBtn->setID("prev-btn");
        menu->addChild(prevBtn);

        auto nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        nextSpr->setFlipX(true);
        nextBtn = CCMenuItemSpriteExtra::create(nextSpr, nullptr, this, menu_selector(WorldsLayer::onPLUSWORLD));
        nextBtn->setPosition({win.width - 50.f, win.height / 2});
        nextBtn->setID("next-btn");
        menu->addChild(nextBtn);

        auto customMaps = CCSprite::createWithSpriteFrameName("GJ_chatBtn_01_001.png");
        auto customMapBtn = CCMenuItemSpriteExtra::create(customMaps, nullptr, this, menu_selector(WorldsLayer::onCM));
        customMapBtn->setPosition({20.f, win.height / 12});
        customMapBtn->setID("custom-btn");
        menu->addChild(customMapBtn);

        this->refreshLvls(0);
        this->schedule(schedule_selector(WorldsLayer::refreshLvls), 2.5f);

        return true;
    }

    void refreshLvls(float) {
        auto win = CCDirector::get()->getWinSize();
        auto menu = static_cast<CCMenu*>(this->getChildByID("world-menu"));
        if (!menu) return;
        auto children = menu->getChildren();
        if (children && children->count()) {
            for (int i = children->count() - 1; i >= 0; --i) {
                auto btn = static_cast<CCNode*>(children->objectAtIndex(i));
                std::string id = btn->getID();
                if (id.rfind("lvl-", 0) == 0) {
                   menu->removeChild(btn, true);
                }
            }
        }

        auto worlds = customTM(win.width, win.height);
        if (worlds.empty()) return;

        if (theWorldWeAreIn >= worlds.size()) theWorldWeAreIn = worlds.size() - 1;
        const auto& world = worlds[theWorldWeAreIn];

        std::string naruto;
        if (c_custom && theWorldWeAreIn < c_customBG.size() && !c_customBG[theWorldWeAreIn].empty()) {
            naruto = (geode::Mod::get()->getConfigDir(true) / "custom" / "backgrounds" / c_customBG[theWorldWeAreIn]).string();
        }

        CCSprite* newBg = nullptr;

        if (!naruto.empty()) {
            newBg = CCSprite::create(naruto.c_str());
        }

        if (!newBg) {
            std::string awesome = "World"_spr + std::to_string(theWorldWeAreIn + 1) + ".png";
            newBg = CCSprite::create(awesome.c_str());
            // alright so it would very funny if custom maps have more worlds then i have worlds for the main game
            // and one of their worlds panics and then falls back to using World99.png or whatever even though it
            // doesnt exist then all you see is darkness :trollface:

            // thank you for reading my useless yapping, have a nice day -jarvisdevil
        }

        if (newBg) {
            newBg->setID("world-bg");
            newBg->setAnchorPoint({0.5f, 0.5f});
            newBg->setPosition({ win.width / 2, win.height / 2 });

            float scaleX = 1.0f;
            float scaleY = 1.0f;
            if (newBg->getContentSize().width > 0 && newBg->getContentSize().height > 0) {
                scaleX = win.width / newBg->getContentSize().width;
                scaleY = win.height / newBg->getContentSize().height;
            }
            newBg->setScaleX(scaleX);
            newBg->setScaleY(scaleY);

            if (auto old = static_cast<CCSprite*>(this->getChildByID("world-bg"))) {
                if (old != newBg) old->removeFromParentAndCleanup(true);
            }
            this->addChild(newBg, -1);
        }

        static_cast<CCLabelBMFont*>(this->getChildByID("world-name"))->setString(world.title.c_str());
        static_cast<CCLabelBMFont*>(this->getChildByID("funny-text"))->setString(world.desc.c_str());

        std::string epicSounds;
        if (c_custom && theWorldWeAreIn < g_customBGM.size() && !g_customBGM[theWorldWeAreIn].empty()) {
            epicSounds = (geode::Mod::get()->getConfigDir(true) / "custom" / "music" / g_customBGM[theWorldWeAreIn]).string();
            // fun fact: i used getConfigDir(true) three times here.
        } else {
            epicSounds = world.audio;
        }

        if (bgmAudio != epicSounds) {
            bgmAudio = epicSounds;
            FMODAudioEngine::sharedEngine()->playMusic(epicSounds.c_str(), true, 1.0f, 0);
        }

        std::vector<WorldLevel> lvls;
        if (c_custom) {
            lvls = world.levels;
        } else {
            lvls = getWorldLevels(win.width, win.height, static_cast<int>(theWorldWeAreIn + 1));
        }

        cocos2d::CCArray* arr = GameLevelManager::get()->getCompletedLevels(false);
        std::set<int> completed;
        for (unsigned i = 0; i < arr->count(); ++i) {
            if (auto g = typeinfo_cast<GJGameLevel*>(arr->objectAtIndex(i))) {
                completed.insert(g->m_levelID);
            }
        }

        bool unlockAll = geode::Mod::get()->getSettingValue<bool>("unlock-all");
        // ^^^ killing "why is the map so hard" comments since 2025!
        bool uhh = false;
        for (size_t i = 0; i < lvls.size(); ++i) {
            const auto& e = lvls[i];
            bool unlocked = unlockAll ||
                (e.order == 1) ||
                (lvls.size() >= static_cast<size_t>(e.order) && completed.count(lvls[e.order - 2].levelID) > 0);

            if (!unlocked) { uhh = true; }
            if (uhh && !unlockAll) { continue; }

            const char* frame = unlocked
                ? "worldLevelBtn_001.png"_spr
                : "worldLevelBtn_locked_001.png"_spr;

            auto spr = CCSprite::createWithSpriteFrameName((std::string(frame)).c_str());
            if (i == lvls.size() - 1) { spr->setScale(2.0f); }
            if (completed.count(e.levelID) > 0) {
                spr->setColor(ccc3(0, 255, 0));
            }

            auto btn = CCMenuItemSpriteExtra::create(spr, nullptr, this, menu_selector(WorldsLayer::onWorld));
            btn->setID("lvl-" + std::to_string(e.levelID));
            btn->setTag(e.levelID);
            btn->setPosition({e.x, e.y});
            btn->setEnabled(unlocked);
            menu->addChild(btn);
        }

        prevBtn->setVisible(theWorldWeAreIn > 0);
        nextBtn->setVisible(theWorldWeAreIn + 1 < worlds.size());
    }

    void onMINUSWORLD(CCObject*) {
        if (theWorldWeAreIn == 0) return;
        theWorldWeAreIn--;
        this->refreshLvls(0);
    }

    void onPLUSWORLD(CCObject*) {
        auto win = CCDirector::get()->getWinSize();
        auto worlds = getWorlds(win.width, win.height);
        if (theWorldWeAreIn + 1 >= worlds.size()) return;

        if (geode::Mod::get()->getSettingValue<bool>("unlock-all")) {
            theWorldWeAreIn++;
            this->refreshLvls(0);
            return;
        }

        if (!ifSpaceUKd(worlds[theWorldWeAreIn].levels)) {
            FLAlertLayer::create("Woah there!", "<cy>Please beat all the levels</c> <cj>before moving on to the next world!</c>", "OK")->show();
            return;
        }
        theWorldWeAreIn++;
        this->refreshLvls(0);
    }

    void onCM(CCObject*) {
        geode::createQuickPopup(
            "Custom Maps",
            "Would you like to open the Custom Maps page?",
            "Sure!",
            "Nah",
            [](auto, bool btn1) {
                if (!btn1) {
                    web::openLinkInBrowser("https://id.jarvisdevil.com/maps/?hello=1");
                }
            }
        );
    }

    bool ifSpaceUKd(const std::vector<WorldLevel>& levels) {
        if (geode::Mod::get()->getSettingValue<bool>("unlock-all")) return true;
        cocos2d::CCArray* arr = GameLevelManager::get()->getCompletedLevels(false);
        std::set<int> completed;
        for (unsigned i = 0; i < arr->count(); ++i)
            if (auto g = typeinfo_cast<GJGameLevel*>(arr->objectAtIndex(i)))
                completed.insert(g->m_levelID);
        for (const auto& e : levels)
            if (!completed.count(e.levelID)) return false;
        return true;
    }

    void onDiscord(CCObject*) {
        web::openLinkInBrowser("https://dsc.gg/devlin");
    }

    void onBack(CCObject*) {
        GameManager::get()->fadeInMenuMusic();
        CCDirector::get()->replaceScene(
            CCTransitionFade::create(0.5f, CreatorLayer::scene())
            // boo hoo, i havent fixed it
        );
    }

    void onWorld(CCObject* sender) {
        int id = static_cast<CCMenuItemSpriteExtra*>(sender)->getTag();
        auto win = CCDirector::get()->getWinSize();
        auto worlds = getWorlds(win.width, win.height);
        for (const auto& w : worlds)
            for (const auto& e : w.levels)
                if (e.levelID == id) {
                    s_delegate.wlvl = e;
                    goto FOUND;
                }
    FOUND:
        GameLevelManager::get()->m_levelDownloadDelegate = &s_delegate;
        GameLevelManager::get()->downloadLevel(id, false);
    }

    static cocos2d::CCScene* scene() {
        auto s = cocos2d::CCScene::create();
        s->addChild(WorldsLayer::create());
        return s;
    }
};

class $modify(TMCreatorLayer, CreatorLayer) {
    bool init() override {
        if (!CreatorLayer::init()) return false;

        if (auto menu = static_cast<CCMenu*>(this->getChildByID("creator-buttons-menu"))) {
            auto mapBtn = static_cast<CCMenuItemSpriteExtra*>(menu->getChildByID("map-button"));
            if (mapBtn) {
                mapBtn->setVisible(false);
            }

            auto worlds = CCSprite::create("worlds.png"_spr);
            worlds->setScale(0.75f);
            auto worldsBtn = CCMenuItemSpriteExtra::create(worlds, nullptr, this, menu_selector(TMCreatorLayer::onAdventureMap));
            worldsBtn->setID("worlds-button");
            worldsBtn->setPosition(mapBtn->getPositionX() + 2.f, mapBtn->getPositionY() - 2.f);
            menu->addChild(worldsBtn);
        }
        return true;
    }

    void onAdventureMap(CCObject* sender) {
        CCDirector::get()->replaceScene(CCTransitionFade::create(0.5f, WorldsLayer::scene()));
    }
};

class $modify(TMLevelInfoLayer, LevelInfoLayer) {
    void onBack(CCObject* sender) {
        if (this->getUserObject("is-world-level"_spr)) CCDirector::get()->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
        else LevelInfoLayer::onBack(sender);
    }
};