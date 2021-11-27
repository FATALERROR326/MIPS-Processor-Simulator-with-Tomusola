#include <iostream>
#include <fstream>
#include <numeric>
#include <stdio.h>
#include <string.h>
#include "RS.hpp"
#include "InstTable.hpp"
#include "Reg.hpp"

using namespace std;

static const int REGISTER_NUM = 10;
static const int MEMORY_LENGTH = 500;

// for both RS type and EXE type
const int INTEGER = 1;
const int INTEGER_LOOP = 2;
const int FU_ADDER_ADD = 3;
const int FU_ADDER_MINUS = 4;
const int FU_MULT_MULT = 5;
const int FU_MULT_DIVID = 6;
const int LOAD = 7;
const int SAVE = 8;

bool hasDependency(RegResStat* regResStat, InstTable* instTable, int instNum) {
    
    int source1 = instTable->insts[instNum].source1;
    int source2 = instTable->insts[instNum].source2;
    bool flag1 = false, flag2 = false;
    //positive is immediate number, negative is register number
    if(source1 < 0) {
        flag1 = regResStat->getFlag(-source1) == 1;
    } else {
        flag1 = false;
    }
    if(source2 < 0) {
        flag2 = regResStat->getFlag(-source2) == 1;
    } else {
        flag2 = false;
    }
    return flag1 || flag2;
}

void loadInstructions(char** insts) {
    //TODO: load instructions into instruction array
}

void decode(InstTable* instTable, string* instructions) {
    //TODO: transfer binary code in instructions to information in instTable->insts
}

int main()
{
    //
    //initializations
    int capacity[5] = {2,2,2,2,2};
    // cycles consumed by each type of execution
    int typeCycle[10] = {1,1,2,3,4,5,6,1,2,2};
    RS *rs = new RS(capacity);
    InstTable *instTable = new InstTable();
    RegResStat *regResStat = new RegResStat(REGISTER_NUM);
    
    int finishNum = instTable->finishNum;
    int stackSize = instTable->stackSize;
    int cycle = 0;
    int endNum = finishNum + 1;
    int* registers = (int*)malloc(sizeof(int) * REGISTER_NUM);
    int* memory = (int*)malloc(sizeof(int) * MEMORY_LENGTH);
    bool haveIssuedThisCycle = true;
    //TODO: manage the input
    
    while(finishNum < stackSize) {
        /* for each cycle */
        cycle ++;
        haveIssuedThisCycle = false;
        //using a sliding window
        for(int instNum = finishNum; instNum < endNum; ++ instNum) {
            /* to operate each line in instruction table
             */
            InstStats inst = instTable->insts[instNum];
            // when rs is not full, issue the new NEXT instruction
            if(!(rs->checkRS(1) == -1) && !haveIssuedThisCycle) {
                instTable->setIssue(cycle + 1, instNum);
                rs->insertRS(1);
                haveIssuedThisCycle = true;
            } else {
                continue;
            }
            //if there is no dependencies, do the execution
            if(instTable->isIssued(cycle, instNum) && !instTable->isExecuted(cycle, instNum) && !hasDependency(regResStat, instTable, instNum)) {
                instTable->setExe(cycle + typeCycle[inst.type], instNum);
            }
            // if execution has been done, do writeback and update register
            if(instTable->isExecuted(cycle, instNum) && !instTable->isWriten(cycle, instNum)){
                instTable->setWb(cycle + 1, instNum);
                if(inst.type == 0) {
                    registers[inst.des] = registers[inst.source1] + registers[inst.source2];
                    regResStat->regFlags[inst.des] = -1;
                }
                
            }
        }
        if(++ endNum > stackSize) {
            -- endNum;
        } else if(!haveIssuedThisCycle) {
            -- endNum;
        }
    }
}
