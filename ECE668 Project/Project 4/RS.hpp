//
//  RS.hpp
//  Project 4
//
//  Created by 张临风 on 2021/11/27.
//  Copyright © 2021 张临风. All rights reserved.
//

#ifndef RS_hpp
#define RS_hpp

#include <stdio.h>

struct Station
{
    int busy;
    //for INTfu, FPadder, FPmult
    int optype;
    int Vj;
    int Vk;
    int Qj;
    int Qk;
    //for load
    int address;
};

class RS
{
public:
    int *NumSta;//[#number of integer FU, FP adder, FP mult, Load, store]
    struct Station *fu;
    
    //struct *Station st;//unaware of storestation now
    RS(int *nums);
    int* getRSnum(void);
    struct Station* getRS(void);
    int checkRS(int type);
    int insertRS(int type);

};
#endif /* RS_hpp */
