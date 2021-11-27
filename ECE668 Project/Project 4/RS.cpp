//
//  RS.cpp
//  Project 4
//
//  Created by 张临风 on 2021/11/27.
//  Copyright © 2021 张临风. All rights reserved.
//

#include "RS.hpp"
#include <numeric>
using namespace std;

RS:: RS(int *nums)
{
    NumSta = nums;
    int all = accumulate(NumSta, NumSta+5, 0);
    struct Station f[all];
    fu = f;
    for (int i = 0; i < all; i ++)
    {
        fu[i].busy = 0;
    }

    /*
    for (int i = 0; i < all; i++)
    {
        Station a{busy:0};
        fu[i] = a;
    }
    */
    //st
}

int* RS:: getRSnum(void)
{
    
    return NumSta;
}

struct Station* RS:: getRS(void)
{
    
    return fu;
}

int RS:: checkRS(int type)
{
    int i = 0;
    int start = 0;
    for (int j = 0; j < type; j++)
    {
        start += NumSta[j];
    }
    //cout <<'.' <<start << endl;
    //cout <<','<< NumSta[type]<<endl;
    int end = start + NumSta[type];
    if (!fu[start].busy) return start;
    for (i = start; i < end; i++)
    {
        //cout << fu[i].busy << endl;
        //cout << i << ','<< endl;
        if (!fu[i].busy)
        {
            //cout << ','<< endl;
            break;
        }
    }
    if (i == start) return -1;
    return i;
}

int RS:: insertRS(int type)
{
    //should also input decoded instruction for fill operands and others.
    int index = checkRS(type);
    if (index != -1)
    {
        //cout << fu[index].busy << '.' << index << endl;
        fu[index].busy = 1;
        //cout << fu[index].busy << ',' << index << endl;
    }
    return index;
}
