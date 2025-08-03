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
        FLAlertLayer::create(
            "Download Failed",
            "Failed to download the level. Please try again later.", 
            "OK"
        )->show();
        GameLevelManager::get()->m_levelDownloadDelegate = nullptr;
    }
};
static inline WorldLvl s_delegate;

class WorldsLayer : public cocos2d::CCLayer {
public:
    CREATE_FUNC(WorldsLayer);

    bool init() override {
        if (!CCLayer::init()) return false;
        auto win = CCDirector::get()->getWinSize();

        auto bg = CCSprite::create("World1.png"_spr);
        bg->setID("world-bg");
        bg->setAnchorPoint({0.5f, 0.5f});
        bg->setPosition({ win.width / 2, win.height / 2 });
        float scaleX = win.width / bg->getContentSize().width;
        float scaleY = win.height / bg->getContentSize().height;
        bg->setScaleX(scaleX);
        bg->setScaleY(scaleY);
        this->addChild(bg, -1);

        auto menu = CCMenu::create();
        menu->setID("world-menu");
        menu->setPosition({0,0});
        addChild(menu);

        auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoBtn_001.png");
        infoSpr->setScale(0.8f);
        auto infoBtn = CCMenuItemSpriteExtra::create(infoSpr, nullptr, this, menu_selector(WorldsLayer::onInfo));
        infoBtn->setID("info-btn");
        infoBtn->setPosition({ win.width - infoSpr->getContentSize().width/2 - 10.f,
                               win.height - infoSpr->getContentSize().height/2 - 10.f });
        menu->addChild(infoBtn);

        auto discordSpr = CCSprite::createWithSpriteFrameName("gj_discordIcon_001.png");
        auto discordBtn = CCMenuItemSpriteExtra::create(discordSpr, nullptr, this, menu_selector(WorldsLayer::onDiscord));
        discordBtn->setID("discord-btn");
        discordBtn->setPosition({
            win.width - discordSpr->getContentSize().width/2 - 13.f,
            discordSpr->getContentSize().height/2 + 20.f
        });
        menu->addChild(discordBtn);

        auto title = CCLabelBMFont::create("W1 - Hello, World!", "bigFont.fnt");
        title->setID("world-name");
        title->setPosition(win.width / 2, win.height - 25.f);
        title->setScale(0.6f);
        this->addChild(title);

        auto funnytext = CCLabelBMFont::create("This is where your adventure starts...", "bigFont.fnt");
        funnytext->setID("funny-text");
        funnytext->setPosition(win.width / 2, win.height / 10);
        funnytext->setScale(0.4f);
        this->addChild(funnytext);

        auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        auto backBtn = CCMenuItemSpriteExtra::create(backSpr, nullptr, this, menu_selector(WorldsLayer::onBack));
        backBtn->setPosition({30.f, win.height - 30.f});
        backBtn->setID("back-btn");
        menu->addChild(backBtn);

        this->refreshLvls(0);
        this->schedule(schedule_selector(WorldsLayer::refreshLvls), 1.0f);

        FMODAudioEngine::sharedEngine()->playMusic("TinyGroove.mp3"_spr, true, 1.0f, 8932);

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

        auto lvls = getWorldLevels(win.width, win.height);
        cocos2d::CCArray* arr = GameLevelManager::get()->getCompletedLevels(false);
        std::set<int> completed;
        for (unsigned i = 0; i < arr->count(); ++i) {
            if (auto g = dynamic_cast<GJGameLevel*>(arr->objectAtIndex(i))) {
                completed.insert(g->m_levelID);
            }
        }
        for (size_t i = 0; i < lvls.size(); ++i) {
            const auto& e = lvls[i];
            bool unlocked = (e.order == 1) ||
                (completed.count(lvls[e.order - 2].levelID) > 0);
            const char* frame = unlocked
                ? "worldLevelBtn_001.png"_spr
                : "worldLevelBtn_locked_001.png"_spr;
            auto spr = CCSprite::createWithSpriteFrameName((std::string(frame)).c_str());
            if (i == lvls.size() - 1) {
                spr->setScale(1.5f);
            }
            auto btn = CCMenuItemSpriteExtra::create(spr, nullptr, this, menu_selector(WorldsLayer::onWorld));
            btn->setID("lvl-" + std::to_string(e.levelID));
            btn->setTag(e.levelID);
            btn->setPosition({e.x, e.y});
            btn->setEnabled(unlocked);
            menu->addChild(btn);
        }
    }

    void onDiscord(CCObject*) {
        web::openLinkInBrowser("https://dsc.gg/devlin");
    }

    void onInfo(CCObject*) {
        auto win = CCDirector::get()->getWinSize();

        if (this->getChildByID("help-me")) {
            this->onClosePromo(nullptr);
            return;
        }

        auto promo = CCSprite::create("promo.png"_spr);
        float scaleX = (win.width * 0.8f) / promo->getContentSize().width;
        float scaleY = (win.height * 0.8f) / promo->getContentSize().height;
        float scale = std::min(scaleX, scaleY);
        promo->setID("help-me");
        promo->setScale(scale);
        promo->setPosition({ win.width / 2, win.height / 2 });
        promo->setZOrder(1000);
        this->addChild(promo, 1000);

        float promoW = promo->getContentSize().width * scale;
        float promoH = promo->getContentSize().height * scale;

        auto xSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
        auto closeBtn = CCMenuItemSpriteExtra::create(xSpr, nullptr, this, menu_selector(WorldsLayer::onClosePromo));
        closeBtn->setPosition({
            promo->getPositionX() - promoW / 2 + xSpr->getContentSize().width * 0.5f - 24.f,
            promo->getPositionY() + promoH / 2 - xSpr->getContentSize().height * 0.5f + 24.f
        });
        closeBtn->setID("promo-close");
        closeBtn->setZOrder(1001);

        auto promoMenu = CCMenu::create();
        promoMenu->setID("promo-menu");
        promoMenu->setPosition({0,0});
        promoMenu->addChild(closeBtn);
        promoMenu->setZOrder(1001);
        this->addChild(promoMenu, 1001);
    }

    void onClosePromo(CCObject*) {
        if (auto promo = this->getChildByID("help-me"))
            promo->removeFromParentAndCleanup(true);
        if (auto promoMenu = this->getChildByID("promo-menu"))
            promoMenu->removeFromParentAndCleanup(true);
    }

    void onBack(CCObject*) {
        GameManager::get()->fadeInMenuMusic();
        CCDirector::get()->replaceScene(
            CCTransitionFade::create(0.5f, CreatorLayer::scene())
        );
    }

    void onWorld(CCObject* sender) {
        int id = static_cast<CCMenuItemSpriteExtra*>(sender)->getTag();
        auto win = CCDirector::get()->getWinSize();
        auto lvls = getWorldLevels(win.width, win.height);
        for (const auto& e : lvls) {
            if (e.levelID == id) {
                s_delegate.wlvl = e;
                break;
            }
        }
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