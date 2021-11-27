//
//  InstTable.hpp
//  Project 4
//
//  Created by 张临风 on 2021/11/27.
//  Copyright © 2021 张临风. All rights reserved.
//

#ifndef InstTable_hpp
#define InstTable_hpp

#include <stdio.h>

/**
    Structure for storing information in each RS row
 */

struct InstStats
{
    int type;
    int source1;
    int source2;
    int des;
    int issueStage;
    int exeStage;
    int wbStage;
    int instNum;
};


/**
    Used to store instructions and their execution stages
 */
class InstTable
{
public:
    int currInstNum = 0;
    int finishNum = 0;
    int stackSize = 0;
    InstStats* insts;
    InstTable(InstStats* insts);
    InstTable();
    void setIssue(int cycle, int instNum);
    void setExe(int cycle, int instNum);
    void setWb(int cycle, int instNum);
    bool isIssued(int cycle, int instNum);
    bool isExecuted(int cycle, int instNum);
    bool isWriten(int instNum, int cycle);
};

#endif /* InstTable_hpp */
