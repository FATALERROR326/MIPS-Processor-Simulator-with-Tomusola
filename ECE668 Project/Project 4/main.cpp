#include <iostream>
#include <fstream>
#include <numeric>
#include <stdio.h>
#include <string.h>
using namespace std;

/**
    Structure for storing information in each RS row
 */
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
};

/**
    Used to store avalibility of registers
 */
class Registers
{
    int* regFlags;
public:
    Registers(int num);
    int getFlag(int regNum);
};


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

InstTable:: InstTable(InstStats* insts)
{
    this->insts = insts;
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

Registers:: Registers(int num)
{
    this->regFlags = (int*)malloc(num * sizeof(int));
}
int Registers:: getFlag(int regNum)
{
    return this->regFlags[regNum];
}

bool hasDependency(Registers* registers, InstTable* instTable) {
    
    registers.getFlag();
}

void display(RS* rs) {
    
}
int main()
{
    //initializations
    int capacity[5] = {2,2,2,2,2};
    RS *rs = new RS(capacity);
    InstTable *instTable = new InstTable();
    Registers *registers = new Registers();
    int finishNum = instTable->finishNum;
    int stackSize = instTable->stackSize;
    int cycle = 0;
    int endNum = finishNum + 1;
    bool haveIssuedThisCycle = true;
    //TODO: manage the input
    
    while(finishNum <= stackSize) {
        /* for each cycle */
        cycle ++;
        haveIssuedThisCycle = false;
        //using a sliding window
        for(int instNum = finishNum; instNum < endNum; ++ instNum) {
            /* to operate each line in instruction table
             */
            // when rs is not full, issue the new NEXT instruction
            if(!rs->isFull() && instTable->isIssued() && !haveIssuedThisCycle) {
                instTable->setIssue(instNum, cycle + 1);
                haveIssuedThisCycle = true;
            } else {
                continue;
            }
            //if there is no dependencies, do the execution
            if(instTable->isIssued() && cycle >= instTable->getIssueCycle() && hasDependency()) {
                
            }
            
        }
        if(++ endNum > stackSize) {
            -- endNum;
        } else if(!haveIssuedThisCycle) {
            -- endNum;
        }
    }
    int index1 = rs->insertRS(1);
    cout << index1 << endl;
}
