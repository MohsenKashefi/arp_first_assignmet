// blackboard.h
#ifndef BLACKBOARD_SECTION_H
#define BLACKBOARD_SECTION_H
#include "../include/const.h"


typedef struct {
    double x;
    double y;
} Spot;

typedef struct {
    Spot drone;
    char pressed_key;
} BlackBoard_Section;

typedef struct {
    double row;
    double column;
    char displaySymbol;
    short displayColor;
    int isActive;
} ChatacterOfApp;


#endif // BLACKBOARD_SECTION_H
