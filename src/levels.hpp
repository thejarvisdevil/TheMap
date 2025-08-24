#pragma once
#include <vector>
#include <string>

struct WorldLevel {
    int levelID;
    float x, y;
    int order;
    std::string authorName;
    int accountID;
    int userID;
};

inline std::vector<WorldLevel> getWorldLevels(float width, float height) {
    return {
        {123562726, width * 0.25f, height * 0.80f - 60.f,  1, "jarvisdevil", 30483751, 257687092},
        {123673148, width * 0.25f, height * 0.65f - 60.f,  2, "jarvisdevil", 30483751, 257687092},
        {124928602, width * 0.35f, height * 0.55f - 60.f,  3, "6Yt", 27334543, 241385219},
        {123699729, width * 0.45f, height * 0.50f - 60.f,  4, "Junotrion", 22572488, 133524459},
        {114869219, width * 0.55f, height * 0.52f - 60.f,  5, "Biskotos", 29373716, 253434464},
        {111864144, width * 0.65f, height * 0.60f - 60.f,  6, "Junotrion", 22572488, 133524459},
        {113000094, width * 0.72f, height * 0.70f - 60.f,  7, "CiPhEn", 28837062, 206414919},
        {119575841, width * 0.65f, height * 0.80f - 60.f,  8, "Biskotos", 29373716, 253434464},
        {113245967,  width * 0.57f, height * 0.70f - 40.f,  9, "ThatPotatoBoi", 22250826, 191954721},
        {123702338, width / 2, height / 2, 10, "jarvisdevil", 30483751, 257687092},
        {123590608, 30.f, height / 9,  11, "BloxinBlocks", 26423130, 236877807},
        {123672485, 60.f, height / 9,  11, "SpaecUK", 34716882, 302615873},
        {113272411, 90.f, height / 9,  11, "rinad2023", 24638848, 219913508},
        {113716144, 120.f, height / 9,  11, "Junotrion", 22572488, 133524459},
        {94122818, 150.f, height / 9,  11, "TechStudent10", 20284359, 179839933}
    };
}