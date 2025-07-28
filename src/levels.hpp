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
        {123590608, width * 0.35f, height * 0.55f - 60.f,  3, "BloxinBlocks", 26423130, 236877807},
        {123699729, width * 0.45f, height * 0.50f - 60.f,  4, "Junotrion", 22572488, 133524459},
        {123672485, width * 0.55f, height * 0.52f - 60.f,  5, "SpaecUK", 34716882, 302615873},
        {111864144, width * 0.65f, height * 0.60f - 60.f,  6, "Junotrion", 22572488, 133524459},
        {113272411, width * 0.72f, height * 0.70f - 60.f,  7, "rinad2023", 24638848, 219913508},
        {113716144, width * 0.65f, height * 0.80f - 60.f,  8, "Junotrion", 22572488, 133524459},
        {94122818,  width * 0.57f, height * 0.70f - 40.f,  9, "TechStudent10", 20284359, 179839933},
        {123702338, width / 2, height / 2, 10, "jarvisdevil", 30483751, 257687092}
    };
}