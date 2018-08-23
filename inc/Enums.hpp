#ifndef ENUMS_HPP
#define ENUMS_HPP

#include <string>
#include <vector>

enum WeaponType
{
    WT_SWORD,
    WT_LANCE,
    WT_AXE,
    WT_TOME,
    WT_ROD,
    WT_BOW,
    WT_PROJECTILE,
    WT_OTHER
};

static const std::vector<std::string> WT_Strings{"Sword", "Lance", "Axe", "Tome", "Rod", "Bow", "Projectile", "Other"};

enum DamageType //UNTOUCHABLE mmm
{
    DT_F,
    DT_M,
    DT_T,
    DT_H,
    DT_ELEMS
};

enum MovementType
{
    MT_WALKING,
    MT_FLYING,
    MT_MOUNTED,
    MT_OTHER
};

static const std::vector<std::string> MT_Strings{"Walking", "Flying", "Mounted", "Other"};

enum UnitAttribute
{
    UA_HP,
    UA_RESIST_F,
    UA_RESIST_M,
    UA_ELEMS
};

static const std::vector<std::string> UA_Strings{"HP", "RES(F)", "RES(M)"};

enum OtherUnitAttribute
{
};

enum UnitState
{
    FIXED,
    DAZED,
    EFFECTS_IMMUNITY,
    GOD,
    US_ELEMS
};

static const std::vector<std::string> US_Strings{"Fixed", "Dazed", "Eff.Inm.", "GOD"};

enum ActionType
{
    AT_MOVE,
    AT_ENEMY,
    AT_ALLY,
    AT_NONE
};

enum TurnPhase
{
    TP_BEGIN,
    TP_SELECTED,
    TP_ACTION,
    TP_ABILITY
};

enum TarjetTeam
{
    TT_ENEMY,
    TT_ALLY
};

static const std::vector<std::string> TT_Strings{"E", "A"};

enum TerrainType
{
    TT_PLAINS,
    TT_FOREST,
    TT_MOUNTAIN,
    TT_WATER,
    TT_WALL,
    TT_FORTRESS
};

enum TerrainAttribute
{
    TA_TYPE,
    TA_WALKABKE,
    TA_FLYABLE,
    TA_MOUNTABLE,
    TA_DEADLY,
    TA_EFFECT
};

#endif