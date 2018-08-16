#ifndef ENUMS_HPP
#define ENUMS_HPP

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

enum DamageType //UNTOUCHABLE
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

enum UnitAttribute
{
    UA_HP,
    UA_RESIST_F,
    UA_RESIST_M,
    UA_ELEMS
};

//std::vector<std::string> UA_Strings = {"HP", "RES(F)", "RES(M)"};

enum OtherUnitAttribute
{
};

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

enum ActionType
{
    AT_MOVE,
    AT_ENEMY,
    AT_ALLY,
    AT_NONE
};

enum TurnPhase
{
    TP_SELECT,
    TP_SELECTED,
    TP_ACTION
};

enum TarjetTeam
{
    TT_ENEMY,
    TT_ALLY
};

#endif