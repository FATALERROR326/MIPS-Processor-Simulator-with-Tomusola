//
//  Reg.hpp
//  Project 4
//
//  Created by 张临风 on 2021/11/27.
//  Copyright © 2021 张临风. All rights reserved.
//

#ifndef Reg_hpp
#define Reg_hpp

#include <stdio.h>

/**
    Used to store avalibility of registers
 */
class RegResStat
{
public:
    int* regFlags;
    RegResStat(int num);
    int getFlag(int regNum);
};
#endif /* Reg_hpp */
