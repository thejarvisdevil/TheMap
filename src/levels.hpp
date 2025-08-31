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

inline std::vector<WorldLevel> getWorldLevels(float width, float height, int worldNum) {
    if (worldNum == 1) return {
        {123562726, width * 0.25f, height * 0.80f - 60.f, 1, "jarvisdevil", 30483751, 257687092},
        {123673148, width * 0.25f, height * 0.65f - 60.f, 2, "jarvisdevil", 30483751, 257687092},
        {124928602, width * 0.35f, height * 0.55f - 60.f, 3, "6Yt", 27334543, 241385219},
        {123699729, width * 0.45f, height * 0.50f - 60.f, 4, "Junotrion", 22572488, 133524459},
        {114869219, width * 0.55f, height * 0.52f - 60.f, 5, "Biskotos", 29373716, 253434464},
        {111864144, width * 0.65f, height * 0.60f - 60.f, 6, "Junotrion", 22572488, 133524459},
        {94012602, width * 0.72f, height * 0.70f - 60.f, 7, "LandonGMD", 7346996, 23394098},
        {119575841, width * 0.65f, height * 0.80f - 60.f, 8, "Biskotos", 29373716, 253434464},
        {113245967, width * 0.57f, height * 0.70f - 40.f, 9, "ThatPotatoBoi", 22250826, 191954721},
        {123702338, width / 2, height / 2, 10, "jarvisdevil", 30483751, 257687092}
    };
    if (worldNum == 2) return {
        {125211367, width * 0.25f, height * 0.75f, 1, "elcolacao14", 16621678, 161786139},
        {104980222, width * 0.50f, height * 0.75f, 2, "Landon72", 15289357, 148184418},
        {125362366, width * 0.75f, height * 0.75f, 3, "Geomaster846", 13835245, 133668844},
        {109566147, width * 0.25f, height * 0.65f, 4, "Landon72", 15289357, 148184418},
        {125253537, width * 0.50f, height * 0.65f, 5, "hexz347", 24448008, 198672284},
        {123392724, width * 0.75f, height * 0.65f, 6, "Landon72", 15289357, 148184418},
        {114684521, width * 0.25f, height * 0.55f, 7, "Landon72", 15289357, 148184418},
        {115512658, width * 0.50f, height * 0.55f, 8, "Landon72", 15289357, 148184418},
        {112452680, width * 0.75f, height * 0.55f, 9, "Landon72", 15289357, 148184418},
        {123974300, width * 0.43f, height * 0.45f, 10, "Rifct", 8927557, 81183402},
        {121733508, width * 0.53f, height * 0.35f, 11, "Rifct", 8927557, 81183402}
    };
    if (worldNum == 3) return {
        {103761026, width * 0.12f, height * 0.28f, 1, "LandonGMD", 7346996, 23394098},
        {104902634, width * 0.28f, height * 0.18f, 2, "LandonGMD", 7346996, 23394098},
        {121110069, width * 0.44f, height * 0.18f, 3, "ZbyndaTheParrot", 28389786, 200147193},
        {112593584, width * 0.60f, height * 0.18f, 4, "LandonGMD", 7346996, 23394098},
        {95352679, width * 0.76f, height * 0.18f, 5, "Rifct", 8927557, 81183402},
        {98899344, width * 0.88f, height * 0.28f, 6, "kwoob", 13593305, 113897789},
        {114705337, width * 0.50f, height * 0.50f, 7, "Cassidyy", 13716622, 132613334}
    };
    if (worldNum == 4) return {
        {118457747, width * 0.22f, height * 0.80f, 1, "Landon72", 15289357, 148184418},
        {111601843, width * 0.38f, height * 0.80f, 2, "Landon72", 15289357, 148184418},
        {120949999, width * 0.62f, height * 0.80f, 3, "CreatorCreepy", 54944, 1181879},
        {115920532, width * 0.78f, height * 0.80f, 4, "Landon72", 15289357, 148184418},
        {122072971, width * 0.22f, height * 0.65f, 5, "Rifct", 8927557, 81183402},
        {116298962, width * 0.38f, height * 0.65f, 6, "kwoob", 13593305, 113897789},
        {90626669, width * 0.62f, height * 0.65f, 7, "kwoob", 13593305, 113897789},
        {49486623, width * 0.78f, height * 0.65f, 8, "LandonGMD", 7346996, 23394098},
        {110437288, width * 0.22f, height * 0.50f, 9, "Junotrion", 22572488, 133524459},
        {81043449, width * 0.38f, height * 0.50f, 10, "CreatorCreepy", 54944, 1181879},
        {95325555, width * 0.62f, height * 0.50f, 11, "CreatorCreepy", 54944, 1181879},
        {93996432, width * 0.78f, height * 0.50f, 12, "kwoob", 13593305, 113897789},
        {122321677, width * 0.50f, height * 0.35f, 13, "Landon72", 15289357, 148184418},
        {118603417, width * 0.50f, height * 0.12f, 14, "Split72", 11839560, 117806273}
    };
    if (worldNum == 5) return {
        {102176589, width * 0.50f, height * 0.20f, 1, "LandonGMD", 7346996, 23394098},
        {92874995, width * 0.72f, height * 0.34f, 2, "LandonGMD", 7346996, 23394098},
        {92874988, width * 0.78f, height * 0.56f, 3, "LandonGMD", 7346996, 23394098},
        {78239200, width * 0.60f, height * 0.72f, 4, "LandonGMD", 7346996, 23394098},
        {73263631, width * 0.32f, height * 0.72f, 5, "LandonGMD", 7346996, 23394098},
        {100599750, width * 0.18f, height * 0.45f, 6, "LandonGMD", 7346996, 23394098},
        {114704144, width * 0.50f, height * 0.50f, 7, "Split72", 11839560, 117806273}
    };
    if (worldNum == 6) return {
        {111065515, width * 0.20f, height * 0.60f, 1, "DubbyBall", 1106735, 4064875},
        {117855782, width * 0.80f, height * 0.55f, 2, "kwoob", 13593305, 113897789},
        {28928859, width * 0.65f, height * 0.40f, 3, "Colon", 106255, 4136576},
        {117683616, width * 0.30f, height * 0.35f, 4, "kwoob", 13593305, 113897789},
        {88664499, width * 0.75f, height * 0.30f, 5, "CreatorCreepy", 54944, 1181879},
        {86907397, width * 0.50f, height * 0.65f, 6, "Rifct", 8927557, 81183402},
        {86369832, width * 0.35f, height * 0.50f, 7, "Rifct", 8927557, 81183402},
        {121057633, width * 0.60f, height * 0.25f, 8, "kwoob", 13593305, 113897789},
        {41577864, width * 0.25f, height * 0.25f, 9, "Colon", 106255, 4136576}
    };
    if (worldNum == 7) return {
        {119200908, width * 0.88f, height * 0.25f, 1, "kwoob", 13593305, 113897789},
        {118238465, width * 0.75f, height * 0.35f, 2, "kwoob", 13593305, 113897789},
        {103502237, width * 0.66f, height * 0.45f, 3, "kwoob", 13593305, 113897789},
        {104574799, width * 0.55f, height * 0.55f, 4, "kwoob", 13593305, 113897789},
        {114315093, width * 0.44f, height * 0.65f, 5, "kwoob", 13593305, 113897789},
        {117731721, width * 0.33f, height * 0.75f, 6, "kwoob", 13593305, 113897789},
        {115364489, width * 0.22f, height * 0.85f, 7, "kwoob", 13593305, 113897789},
        {92720140, width * 0.35f, height * 0.38f, 8, "Rifct", 8927557, 81183402}
    };
    if (worldNum == 8) return {
        {65583693, width * 0.20f, height * 0.86f, 1, "LandonGMD", 7346996, 23394098},
        {103378418, width * 0.50f, height * 0.70f, 2, "LandonGMD", 7346996, 23394098},
        {121660466, width * 0.80f, height * 0.86f, 3, "Rifct", 8927557, 81183402},
        {120475504, width * 0.86f, height * 0.70f, 4, "kwoob", 13593305, 113897789},
        {74677800, width * 0.76f, height * 0.50f, 5, "LandonGMD", 7346996, 23394098},
        {103919159, width * 0.76f, height * 0.30f, 6, "Rifct", 8927557, 81183402},
        {105786587, width * 0.80f, height * 0.14f, 7, "Rifct", 8927557, 81183402},
        {119663025, width * 0.50f, height * 0.14f, 8, "kwoob", 13593305, 113897789},
        {114397040, width * 0.20f, height * 0.14f, 9, "Rifct", 8927557, 81183402},
        {93846197, width * 0.14f, height * 0.30f, 10, "Rifct", 8927557, 81183402},
        {87096957, width * 0.24f, height * 0.50f, 11, "LandonGMD", 7346996, 23394098},
        {72293051, width * 0.14f, height * 0.70f, 12, "Cassidyy", 13716622, 132613334},
        {96327089, width * 0.50f, height * 0.50f, 13, "Cassidyy", 13716622, 132613334}
    };
    if (worldNum == 9) return {
        {125411607, width * 0.60f, height * 0.60f, 1, "jarvisdevil", 30483751, 257687092},
        {125411612, width * 0.40f, height * 0.40f, 2, "jarvisdevil", 30483751, 257687092},
        {112208643, width * 0.50f, height * 0.50f, 3, "jarvisdevil", 30483751, 257687092}
    };
    return {};
}

struct World {
    std::string title;
    std::string desc;
    std::string audio;
    std::vector<WorldLevel> levels;
};

inline std::vector<World> getWorlds(float width, float height) {
    return {
        {
            "W1 - Hello, World!",
            "This is where your adventure starts...",
            "TinyGroove.mp3"_spr,
            getWorldLevels(width, height, 1)
        },
        {
            "W2 - Blue Sky",
            "Something blue or high in the sky...",
            "RowdyCloudy.mp3"_spr,
            getWorldLevels(width, height, 2)
        },
        {
            "W3 - Water World",
            "It's just water!",
            "Aquatica.mp3"_spr,
            getWorldLevels(width, height, 3)
        },
        {
            "W4 - AWESOME COLORS!!",
            "WOOOOOOOOOO!!! COLORS!!!",
            "Disasteroids.mp3"_spr,
            getWorldLevels(width, height, 4)
        },
        {
            "W5 - Poison Ivy",
            "I think you'll survive. (hopefully)",
            "ResearchNDevelopment.mp3"_spr,
            getWorldLevels(width, height, 5)
        },
        {
            "W6 - GETTING WEIRD",
            "I found some levels for ya!",
            "Breakanoids.mp3"_spr,
            getWorldLevels(width, height, 6)
        },
        {
            "W7 - Red World",
            "(do you get the joke?)",
            "Purgatory.mp3"_spr,
            getWorldLevels(width, height, 7)
        },
        {
            "W8 - Darkness",
            "It's a little bit dark in here...",
            "HeckOnWheels.mp3"_spr,
            getWorldLevels(width, height, 8)
        },
        {
            "Epilogue",
            "",
            "JarvisdevilsStaticSound.mp3"_spr,
            getWorldLevels(width, height, 9)
        }
    };
}