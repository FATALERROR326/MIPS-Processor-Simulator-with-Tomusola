//
//  Reg.cpp
//  Project 4
//
//  Created by 张临风 on 2021/11/27.
//  Copyright © 2021 张临风. All rights reserved.
//

#include "Reg.hpp"
#include <stdlib.h>
RegResStat:: RegResStat(int num)
{
    this->regFlags = (int*)malloc(num * sizeof(int));
    for(int i = 0; i < num; ++ i) {
        regFlags[i] = -1; //-1 stands for available register
    }
}
int RegResStat:: getFlag(int regNum)
{
    return this->regFlags[regNum];
}
