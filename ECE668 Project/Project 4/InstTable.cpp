//
//  InstTable.cpp
//  Project 4
//
//  Created by 张临风 on 2021/11/27.
//  Copyright © 2021 张临风. All rights reserved.
//

#include "InstTable.hpp"

InstTable:: InstTable(InstStats* insts)
{
    this->insts = insts;
}

InstTable:: InstTable()
{
}

void InstTable:: setIssue(int cycle, int instNum)
{
    this->insts[instNum].issueStage = cycle;
}

void InstTable:: setExe(int cycle, int instNum)
{
    this->insts[instNum].exeStage = cycle;
}

void InstTable:: setWb(int cycle, int instNum)
{
    this->insts[instNum].wbStage = cycle;
}

bool InstTable:: isIssued(int cycle, int instNum) {
    if(this->insts[instNum].issueStage > 0 && this->insts[instNum].issueStage <= cycle) {
        return true;
    }
    return false;
}

bool InstTable:: isExecuted(int instNum, int cycle) {
    if(this->insts[instNum].exeStage > 0 && this->insts[instNum].exeStage <= cycle) {
        return true;
    }
    return false;
}

bool InstTable:: isWriten(int instNum, int cycle) {
    if(this->insts[instNum].wbStage > 0 && this->insts[instNum].wbStage <= cycle) {
        return true;
    }
    return false;
}
