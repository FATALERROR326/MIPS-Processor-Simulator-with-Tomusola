/*************************************************************************
    > File Name: MIPSsim.cpp
    > Author: Linfeng
    > Mail: linfengzhang@umass.edu
    > Created Time: 一 12/ 9 20:55:14 2021
 ************************************************************************/
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<fstream>
#include<sstream>
#include<iomanip>
using namespace std;
#define INSL 32
#define HINSL 16
#define ML 2048
#define PATH "mips.txt"


class Trans{
private:
    unsigned int *array;
    int length;
public:
    Trans(unsigned int* array,int length)
    {
        this->array=array;
        this->length=length;
    }
    unsigned long int trans(int start, int end)
    {
        unsigned long int num=0;
        if(start>=end)
        {
            printf("Error: the start must be smaller than the end");
        }
        else if(start<0||end>length)
        {
            printf("Error: please enter the correct range");
        }
        else
        {
            int scale=1;
            for(int i=end;i>=start;i--)
            {
                num+=array[i]*scale;
                scale*=2;
            }
        }
        return num;
    }
    void fill(string str)
    {
        int temp=0;
        for(int i=31;i>=0;i--)
        {
            temp=(int)(str[i]-48);
            array[length+i]=temp;
        }
        length+=INSL;
    }
        
    
};
class InstMem{
private:
    unsigned int ins[ML]={0};
    int max=0;
    bool isBeyond=false;
public:
    InstMem(string path)
    {
        ifstream ifs;
        Trans tr=Trans(ins,0);
        ifs.open(path);
        if(!ifs)
            cout<<"***Error: cannot read or find the file with file path: "<<PATH<<"."<<endl;
        string str;
        while(!ifs.eof())
        {
            getline(ifs,str);
            tr.fill(str);
            max++;
        }
        ifs.close();
    }
    int getInsN()
    {
        return max;
    }
    unsigned int* getIns(int pos)
    {
        if(pos/4+1>max)
        {
            isBeyond=true;
        }
        unsigned int* p=&ins[pos*8];
        return p;
    }
    void display()
    {
        cout<<"Values in instruction memory: "<<endl;
        for(int i=0;i<max;i++)
        {
            cout<<"index: "<<i<<", "<<"number: "<<ins[i]<<endl;
        }
    }
    bool isExhausted()
    {
        return isBeyond;
    }
    
};
class RegArr{
private:
    unsigned int reg[32]={0};
public:
    unsigned int get(int index)
    {
        return reg[index];
    }
    void save(int index,int num)
    {
        reg[index]=num;
    }
    void saveMul(int index, long int num)
    {
        reg[index]=(int)num;
        reg[index+1]=(int)(num>>INSL);
    }
    void display()
    {
        cout<<"\n----------------------------\n";
        cout<<"Values in register array: "<<endl;
        for(int i=0;i<=32;i++)
        {
            if(reg[i]!=0)
            {
                cout<<dec<<uppercase<<"$";
                printf("%d:       ",i);
                if(i<=9) cout<<" ";
                printf("%#.8X\n",reg[i]);
            }
        }
        cout<<"----------------------------\n";
    }
};
class PLReg{
private:
    unsigned int type;
    unsigned int ALUop;
    unsigned int Rs;
    unsigned int Rt;
    unsigned int Rd;
    unsigned int imme;
    unsigned int* ins;
    unsigned long long int ALUResult;
    bool rtd;
    bool readM;
    bool writeM;
    bool zf;
public:
    PLReg(unsigned int type)
    {
        this->type=type;
        this->ALUop=0;
        this->Rs=0;
        this->Rt=0;
        this->Rd=0;
        this->imme=0;
        this->ins=0;
        this->ALUResult=0;
        this->rtd=false;
        this->readM=false;
        this->writeM=false;
        this->zf=false;
    }
    void setType(int type)
    {
        this->type=type;
    }
    void setALUop(unsigned int ALUop)
    {
        this->ALUop=ALUop;
    }

    void setRs(unsigned int Rs)
    {
        this->Rs=Rs;
    }
    void setRt(unsigned int Rt)
    {
        this->Rt=Rt;
    }
    void setImme(unsigned int imme)
    {
        this->imme=imme;
    }
    void setRd(unsigned int Rd)
    {
        this->Rd=Rd;
    }
    void setALUResult(unsigned long int ALUResult)
    {
        this->ALUResult=ALUResult;
    }
    void setIns(unsigned int* ins)
    {
        this->ins=ins;
    }
    void setRtd(bool rtd)
    {
        this->rtd=rtd;
    }
    void setReadM(bool readM)
    {
        this->readM=readM;
    }
    void setWriteM(bool writeM)
    {
        this->writeM=writeM;
    }
    void setZf(bool zf)
    {
        this->zf=zf;
    }
    unsigned int getALUop()
    {
        return ALUop;
    }
    unsigned int getRs()
    {
        return Rs;
    }
    unsigned int getImme()
    {
        return imme;
    }
    unsigned int getRt()
    {
        return Rt;
    }
    unsigned int getRd()
    {
        return Rd;
    }
    unsigned int* getIns()
    {
        return ins;
    }
    unsigned long int getALUResult()
    {
        return ALUResult;
    }
    bool getRtd()
    {
        return rtd;
    }
    bool getReadM()
    {
        return readM;
    }
    bool getWriteM()
    {
        return writeM;
    }
    bool getZf()
    {
        return zf;
    }
    void display()
    {
        cout<<"\n-------------------------\n";
        switch (type) {
            case 0:
                
                cout<<"instruction(s) stored in IF/ID register:"<<endl;
                for(int i=0;i<INSL;i++)
                {
                    cout<<*(ins+i);
                }
                cout<<endl;
                break;
            case 1:
                cout<<"Value stored in ID/EX register:"<<endl;
                cout<<"ALUop: 0x"<<hex<<ALUop<<endl;
                cout<<"Rs: 0x"<<hex<<Rs<<endl;
                cout<<"Rt: 0x"<<hex<<Rt<<endl;
                cout<<"Rd: 0x"<<hex<<Rd<<endl;
                cout<<"imme: 0x"<<hex<<imme<<endl;
                cout<<"Rtd: "<<rtd<<endl;
                break;
            case 2:
                cout<<"Value stored in EX/MEM register:"<<endl;
                cout<<"ALUResult: 0x"<<hex<<ALUResult<<endl;
                cout<<"readMem: "<<readM<<endl;
                cout<<"WriteMem: "<<writeM<<endl;
                //cout<<"imme: "<<imme<<endl;
                break;
            case 3:
                cout<<"Value stored in MEM/WB register:"<<endl;
                if(readM)
                {
                    cout<<"Data from Mem to Reg: 0x"<<hex<<ALUResult<<endl;
                }
                else if(!writeM)
                {
                    cout<<"Data from ALU to Reg: 0x"<<hex<<ALUResult<<endl;
                }
                else
                {
                    cout<<"No write-backs"<<endl;
                }
                break;
            default:
                cout<<"***Error: get content failure with unknown Pipeline Register type code:"<<type<<"."<<endl;
                break;
        }
        cout<<"-------------------------\n";
    }

};
//add:0 sub:1 and:2 or:3 sll:4 srl:5 mul:6 lui:7 SLTI:8 BEQ:9
class Decoder{
private:
    unsigned int buffer[INSL];
    unsigned int op=0;
    bool isIn(int n,int *array)
    {
        for(int i=0;i<4;i++)
        {
            if(array[i]==n)
                return true;
        }
        return false;
    }
    int signEx(unsigned short imme)
    {
        unsigned int a;
        unsigned int t=imme>>15;
        if(t==1)
        {
            a=((int)imme|0xFFFF0000);
            return a;
        }
        else
        {
            a=((int)imme|0x00000000);
            return a;
        }
    }
    int logEx(unsigned short imme)
    {
        return (int)imme|0x00000000;
    }
public:
    Decoder(unsigned int* inst)
    {
        memcpy(buffer,inst,INSL*sizeof(int));
    }
    PLReg* execute()
    {
        Trans tr= Trans(buffer,INSL);
        op=(int)tr.trans(0,5);
        PLReg temp= PLReg(1);

        if(op==0||op==28)//if R-type
        {
            
            int func=(int)tr.trans(26,31);
            if(op==28||(func!=0&&func!=2))
            {
                temp.setRs((int)tr.trans(6,10));
            }
            else
            {
                temp.setRs((int)tr.trans(21,25));
            }
            temp.setRt((int)tr.trans(11,15));
            temp.setRd((int)tr.trans(16,20));
            temp.setRtd(true);
            if(op==28)
                temp.setALUop(6);
            else{
                if(func==32)
                {

                    temp.setALUop(0);
                }
                else if(func==34)
                {

                    temp.setALUop(1);
                }
                else if(func==36)
                {

                    temp.setALUop(2);
                }
                else if(func==37)
                {

                    temp.setALUop(3);
                }
                else if(func==0)
                {
                    temp.setALUop(4);

                }
                else if(func==2)
                {
                    temp.setALUop(5);
                }
                else
                {
                    cout<<"***Error: unknown R-type instruction with function code:"<<func<<"."<<endl;
                }
            }
        }
        //if I-type
        else if(op==8||op==35||op==43||op==4||op==15||op==13||op==10||op==11||op==12)
        {
            unsigned short imme=tr.trans(16,31);
            temp.setRs((int)tr.trans(6,10));
            temp.setRt((int)tr.trans(11,15));
            temp.setRtd(false);
            if(op==15)
            {
                temp.setALUop(7);
            }
            else if(op==8||op==35||op==43)
            {
                temp.setALUop(0);
            }
            else if(op==4)
            {
                temp.setALUop(9);
            }
            else if(op==10||op==11)
            {
                temp.setALUop(8);
            }
            else if(op==12)
            {
                temp.setALUop(2);
            }
            else if(op==13)
            {
                temp.setALUop(3);
            }
            if(op==4||op==43||op==8||op==35||op==15||op==13||op==10||op==11||op==12)
            {
                if(op==10)
                    temp.setImme(signEx(imme));
                else
                    temp.setImme(logEx(imme));
            }
            if(op==35)
            {
                temp.setReadM(true);
                temp.setWriteM(false);
            }
            else if(op==43)
            {
                temp.setReadM(false);
                temp.setWriteM(true);
            }
        }
        else
        {
            cout<<"***Error: unexpected instruction with operation code:"<<op<<"."<<endl;
        }
        
            
        PLReg* p;
        p=&temp;
        return p;
    }
    bool isHazard(PLReg temp, int* t, int* d)
    {
        //We have put the value of shamt into rs and don't need to modify it.
        if(temp.getALUop()==4||temp.getALUop()==5)//SLL SRL
        {
            if(isIn(temp.getRt(),t))
            {
                *d=temp.getRt();
                return true;
            }
        }
        else if(op==0||op==28)//R-type
        {
            if(isIn(temp.getRs(),t))
            {
                *d=temp.getRs();
                return true;
            }
            if(isIn(temp.getRt(), t))
            {
                *d=temp.getRt();
                return true;
            }
        }
        else if(temp.getALUop()!=7)//I-type
        {
            if(((op==43||op==4)&&(isIn(temp.getRt(), t))))
            {
                *d=temp.getRt();
                return true;
                
            }
            if(isIn(temp.getRs(), t))
            {
                *d=temp.getRs();
                return true;
            }
        }
        return false;
    }
    PLReg getRegN(PLReg temp, RegArr reg)
    {
        //We have put the value of shamt into rs and don't need to modify it.
        if(temp.getALUop()==4||temp.getALUop()==5)//SLL SRL
        {
            temp.setRt(reg.get(temp.getRt()));
        }
        else if(op==0||op==28)//R-type
        {
            temp.setRs(reg.get(temp.getRs()));
            temp.setRt(reg.get(temp.getRt()));
        }
        else if(temp.getALUop()!=7)//I-type
        {
            if(op==43||op==4)
                temp.setRt(reg.get(temp.getRt()));
            temp.setRs(reg.get(temp.getRs()));
        }
        return temp;
    }
};
class ALU{
private:
    PLReg tReg=PLReg(2);
    RegArr* regarr;
    unsigned int ADD(int a,int b)
    {
        return a+b;
    }
    unsigned int SUB(int a,int b)
    {
        return a-b;
    }
    unsigned int AND(int a,int b)
    {
        return a&b;
    }
    unsigned int OR(int a,int b)
    {
        return a|b;
    }
    unsigned int SLL(int a,int b)
    {
        return a<<b;
    }
    unsigned int SRL(int a,int b)
    {
        return a>>b;
    }
    double MUL(double a,double b)
    {
        return a*b;
    }
    unsigned int SLTI(int a,int b)
    {
        return a<b;
    }
public:
    ALU(PLReg idReg,RegArr* regarr)
    {
        this->tReg=idReg;
        this->regarr=regarr;
    }
    PLReg execute()
    {
        switch(tReg.getALUop())
        {
            case 0:
                if(tReg.getRtd()==true)
                    tReg.setALUResult(ADD(tReg.getRs(),tReg.getRt()));
                else
                    tReg.setALUResult(ADD(tReg.getRs(),tReg.getImme()));
                break;
            case 1:
                tReg.setALUResult(SUB(tReg.getRs(),tReg.getRt()));
                break;
            case 2:
                if(tReg.getRtd()==true)
                    tReg.setALUResult(AND(tReg.getRs(),tReg.getRt()));
                else
                    tReg.setALUResult(AND(tReg.getRs(),tReg.getImme()));
                break;
            case 3:
                if(tReg.getRtd()==true)
                    tReg.setALUResult(OR(tReg.getRs(),tReg.getRt()));
                else
                    tReg.setALUResult(OR(tReg.getRs(),tReg.getImme()));
                break;
            case 4:
                tReg.setALUResult(SLL(tReg.getRt(),tReg.getRs()));
                break;
            case 5:
                tReg.setALUResult(SRL(tReg.getRt(),tReg.getRs()));
                break;
            case 6:
                tReg.setALUResult(MUL(tReg.getRs(),tReg.getRt()));
                break;
            case 7:
                tReg.setALUResult((unsigned int)SLL(tReg.getImme(), HINSL));
                break;
            case 8:
                tReg.setALUResult(SLTI(tReg.getRs(), tReg.getImme()));
                break;
            case 9:
                tReg.setALUResult(SUB(tReg.getRs(),tReg.getRt()));
                if(tReg.getALUResult()==0)
                    tReg.setZf(true);
                else
                    tReg.setZf(false);
                break;
            default:
                cout<<"***Error: unexpected ALU operation with ALUop:"<<tReg.getALUop()<<"."<<endl;
        }
        return tReg;
    }
};
class DataMem{
private:
    int max=-1;
    int data[ML]={0};
public:
    PLReg execute(PLReg inReg)
    {
        PLReg temp=inReg;
        if(temp.getReadM())
        {
            temp.setALUResult(get((int)temp.getALUResult()));
        }
        else if(temp.getWriteM())
        {
            save((int)temp.getALUResult(), temp.getRt());
        }
        return temp;
    }
    int get(int pos)
    {
        return data[pos];
    }
    void save(int pos,int num)
    {
        max=max>pos?max:pos;
        data[pos]=num;
    }
    void display()
    {
        cout<<"\n----------------------------\n";
        cout<<"Values in data memory: "<<endl;
        if(max==-1) cout<<"None"<<endl;
        else{
            for(int i=0;i<=max;i++)
            {
                if(data[i]!=0)
                {
                    cout<<hex<<uppercase<<"index: "<<setw(2);
                    printf("%#.3X",i);
                    cout<<", "<<"number: ";
                    printf("%#.8X\n",data[i]);
                }
            }
        }
        cout<<"----------------------------\n";
    }
    
};
class WriteBacker{
public:
    void execute(PLReg inReg, RegArr* ra)
    {
        if(!inReg.getWriteM()&&(inReg.getALUop()!=9))
        {
            if(!inReg.getRtd())
            {
                ra->save(inReg.getRt(), (int)inReg.getALUResult());
            }
            else
            {
                if(inReg.getALUop()==6)
                {
                    ra->saveMul(inReg.getRd(), inReg.getALUResult());
                }
                else
                    ra->save(inReg.getRd(), (int)inReg.getALUResult());
            }
        }
    }
};
class Buffer{
private:
    int offset;
    bool zf;
    int opcode;
public:
    Buffer()
    {
        offset=0;
        zf=false;
        opcode=0;
    }
    void setInfo(PLReg reg)
    {
        this->offset=reg.getImme();
        this->zf=reg.getZf();
        this->opcode=reg.getALUop();
    }
    bool isBranch()
    {
        return (zf==true)&&(opcode==9);
    }
    int getOffset()
    {
        return offset;
    }
    void setNull()
    {
        offset=0;
        zf=false;
        opcode=0;
    }
};
class PC{
private:
    int counter;
public:
    PC()
    {
        counter=-4;
    }
    int getAddr()
    {
        counter+=4;
        return counter;
    }
    void execute(Buffer* bf)
    {
        if(bf->isBranch())
        {
            counter+=bf->getOffset()*4;
            bf->setNull();
        }
    }
    int getInsN()
    {
        return counter/4+1;
    }
};
class Scheduler{
private:
    int paraNum;
    int mode;
    PC pc=PC();
    DataMem dm=DataMem();
    RegArr ra=RegArr();
    InstMem im=InstMem(PATH);
    Buffer bf=Buffer();
    class Controller{
    private:
        int arr[5]={0};
        int ins=0;
        int cycle=0;
        int pos=0;
        int outN=0;
        bool isOut=false;
        bool close=false;
        bool increase=true;
        int buffer[2];
        int shift(int in)
        {
            if(arr[4]==1)
            {
                isOut=true;
                outN++;
            }
            arr[4]=arr[3];
            arr[3]=arr[2];
            arr[2]=arr[1];
            arr[1]=arr[0];
            arr[0]=in;
            int t=in;
            return t;
        }
    public:
        int* execute(int mode)
        {
            if(increase&&!close)
            {
                ins+=shift(1);
            }
            else
            {
                ins+=shift(0);
                
            }
            this->cycle++;
            if(mode==1)
            {
                cout<<dec<<"The ";
                printf("%.3d",cycle);
                cout<<"th cycle:";
                
            }
            if(mode==0&&increase)
            {
                if(!close)
                {
                    cout<<endl<<dec<<"Load "<<ins<<"th instruction."<<endl;
                    cout<<"I F|I D|E X|MEM|W B|EXECUTED"<<endl;
                }
            }
            for(int i=0;i<5;i++)
            {
                if(i<pos)
                {
                    if(i==0)
                        cout<<"[";
                    cout<<dec<<buffer[i];
                    if(i==pos-1)
                        cout<<"] ";
                    else if(pos==2)
                        cout<<"   ";
                    
                }
                else
                {
                    cout<<dec<<" "<<arr[i];
                    if(i!=4)cout<<"  ";
                    else cout<<" ";
                }
            }
            for(int i=0;i<cycle-5;i++)
            {
                if(i==0)
                    cout<<"|";
                else if(i%10==0)
                    cout<<" ";
                cout<<"·";
            }
            cout<<endl;
            if(isOut)
            {
                //cout<<dec<<"The "<<outN<<"th instruction has been completed."<<endl;
                isOut=false;
            }
            return arr;
            
        }
        void prepareClose()
        {
            close=true;
        }
        void cutdown()
        {
            increase=false;
        }
        void backToWork()
        {
            increase=true;
        }
        void flush(int pos)
        {
            this->pos=pos;
            for(int i=0;i<pos;i++)
            {
                buffer[i]=arr[i];
                arr[i]=0;
            }
        }
        void flushBack()
        {

            for(int i=0;i<pos;i++)
            {
                arr[i]=buffer[i];
            }
            
            pos=0;
        }
        int getIns()
        {
            return ins;
        }
        int getCyc()
        {
            return this->cycle;
        }
        bool toClose()
        {
            if(close&&arr[0]==0&&arr[1]==0&&arr[2]==0&&arr[3]==0&&arr[4]==0)
            {
                return true;
            }
            else return false;
        }
        void open()
        {
            close=false;
        }
        bool isPrepare()
        {
            return close;
        }
        
    };
public:
    void run()
    {
        char ch=0;
        while(1)
        {
            cout<<"\nChoose a running mode: (I)nstruction, (C)ycle."<<endl;
            ch=getchar();
            int num;
            if(ch=='i'||ch=='I')
            {
                mode=0;
                cout<<"\nThere are totally "<<im.getInsN()<<" instructions in Instruction Memory.";
                while(1)
                {
                    cout<<"\nHow many instruction(s) to run?"<<endl;
                    cin>>num;
                    if(num<=im.getInsN())
                        break;
                    else
                        cout<<"\nPlease enter the correct number!"<<endl;
                }
                getchar();
                cout<<"\n***SIMULATION STARTED***"<<endl;
                simulate(mode, num);
                break;
            }
            else if(ch=='c'||ch=='C')
            {
                mode=1;
                cout<<"\nHow many cycles to run?"<<endl;
                cin>>num;
                getchar();
                cout<<"\n***SIMULATION STARTED***"<<endl;
                simulate(mode, num);
                break;
            }
            else
                cout<<"\nPlease enter an expected letter!"<<endl;
        }
            
        
    }
    void simulate(int mode,int num)
    {
        PLReg ifidReg=PLReg(0);
        PLReg idexReg=PLReg(1);
        PLReg exmemReg=PLReg(2);
        PLReg memwbReg=PLReg(3);
        bool brk=false;
        int it=0,cyc=0;
        int* ctrArr;
        int accessArr[5]={0};
        int des[4]={-1};
        int d=-1;
        bool branchClose=false;
        char ch=0;
        Controller ct=Controller();
        while(1)
        {
            if(mode==0)
                ct.open();
            do{
                PLReg wbreg=PLReg(3);
                PLReg exreg=PLReg(2);
                PLReg idreg=PLReg(1);
                PLReg ifreg=PLReg(0);
                for(int i=0;i<4;i++)
                    des[i]=-1;
                ctrArr=ct.execute(mode);
                if(ctrArr[4]==1)
                {
                    bool isClHaz=false;
                    if(!memwbReg.getWriteM()&&(memwbReg.getALUop()!=9))
                    {
                        if(!memwbReg.getRtd())
                        {
                            if(memwbReg.getRt()==d)
                                isClHaz=true;
                        }
                        else
                        {
                            if(memwbReg.getRd()==d)
                                isClHaz=true;
                            
                        }
                            
                    }
                    if(isClHaz)
                    {
                        ct.backToWork();
                        ct.flushBack();
                        accessArr[1]--;
                        d=-1;
                    }
                    WriteBacker wb=WriteBacker();
                    wb.execute(memwbReg,&ra);
                    accessArr[4]++;
                }
                if(ctrArr[3]==1)
                {
                    wbreg=dm.execute(exmemReg);
                    wbreg.setType(3);
                    accessArr[3]++;
                    if(!wbreg.getWriteM()&&(wbreg.getALUop()!=9))
                    {
                        if(!wbreg.getRtd())
                        {
                            des[2]=wbreg.getRt();
                        }
                        else
                        {
                            if(wbreg.getALUop()==6)
                            {
                                des[2]=wbreg.getRd();
                                des[3]=wbreg.getRd()+1;
                            }
                            else
                                des[2]=wbreg.getRd();
                        }
                    }
                    memwbReg=wbreg;
                }
                if(ctrArr[2]==1)
                {
                    ALU alu=ALU(idexReg,&ra);
                    exreg=alu.execute();
                    bf.setInfo(exreg);
                    if(bf.isBranch()&&(bf.getOffset()+pc.getInsN()+1>im.getInsN()))
                    {
                        cout<<"\n***Out of some branches in the end of instruction(s), the instruction(s) have been exhausted.\n***The simulation will be terminated soon.\n"<<endl;
                        ctrArr[0]=0;
                        ct.prepareClose();
                        branchClose=true;
                    }
                    if(exreg.getALUop()==9)
                    {
                        ct.flushBack();
                        ct.backToWork();
                    }
                    exreg.setType(2);
                    accessArr[2]++;
                    if(!exreg.getWriteM()&&(exreg.getALUop()!=9))
                    {
                        if(!exreg.getRtd())
                        {
                            des[0]=exreg.getRt();
                        }
                        else
                        {
                            if(exreg.getALUop()==6)
                            {
                                des[0]=exreg.getRd();
                                des[1]=exreg.getRd()+1;
                            }
                            else
                                des[0]=exreg.getRd();
                        }
                    }
                    exmemReg=exreg;
                }
                if(ctrArr[1]==1)
                {
                    Decoder dc=Decoder(ifidReg.getIns());
                    idreg=(*dc.execute());
                    int *p = &d;
                    if(dc.isHazard(idreg, des, p))
                    {
                        ct.flush(2);
                        ct.cutdown();
                    }
                    else if(idreg.getALUop()==9)
                    {
                        ct.flush(1);
                        ct.cutdown();
                    }
                    idreg=(dc.getRegN(idreg, ra));
                    
                    accessArr[1]++;
                    idexReg=idreg;
                }
                if(ctrArr[0]==1)
                {
                    pc.execute(&bf);
                    ifreg=PLReg(0);
                    ifreg.setIns(im.getIns(pc.getAddr()));
                    accessArr[0]++;
                    if(!im.isExhausted())
                        ifidReg=ifreg;
                    else
                        ifreg=ifidReg;
                }
                if(mode==0)it=ct.getIns();
                else cyc=ct.getCyc();
                
                
                if(it>=num)
                    ct.prepareClose();
                else if(im.isExhausted())
                {
                    if(!ct.isPrepare()){
                        if(mode==0)
                            cout<<"\n***Out of some branches, the instuctions have been exhuasted.\n***The simulation will be terminated soon.\n";
                        else
                            cout<<"\n***The instuctions have been exhuasted.\n***The simulation will be terminated soon.\n";
                    }
                    ct.prepareClose();
                }
                
            }
            while(!ct.toClose()&&cyc<num);
            if(!im.isExhausted()&&!branchClose)
            {
                cout<<"\n***SIMULATION STOPPED***\n"<<endl;
                while(true)
                {
                    cout<<pc.getInsN()<<" instruction(s) have been executed or jumped";
                    if(im.getInsN()-pc.getInsN()==0)
                    {
                        cout<<".";
                        brk=true;
                        break;
                    }
                    cout<<" and "<<im.getInsN()-pc.getInsN()<<" instruction(s) can be executed.";
                    cout<<"\nWould you like to continue simulation? (C)ontinue, (Q)uit"<<endl;
                    ch=getchar();
                    if(ch=='C'||ch=='c')
                    {
                        it=0;
                        cyc=0;
                        if(mode==0)
                        {
                            cout<<"\nHow many instruction(s) to run:"<<endl;
                        }
                        else if(mode==1)
                        {
                            cout<<"\nHow many cycles to run:"<<endl;
                        }
                        int n=0;
                        if(mode==0)
                        {
                            while(true)
                            {
                                cin>>n;
                                if(n>im.getInsN()-pc.getInsN())
                                    cout<<"\nPlease enter the correct number!\n";
                                else
                                    break;
                            }
                            
                        }
                        else{
                            cin>>n;
                        }
                        getchar();
                        num+=n;
                        cout<<"\n***SIMULATION CONTINUED***"<<endl;
                        break;
                    }
                    else if(ch=='Q'||ch=='q')
                    {
                        brk=true;
                        break;
                    }
                    else
                        cout<<"\nPlease enter the expected letter!"<<endl;
                }
            }
            else
                break;
            if(brk)
                break;
        }
        
        int totalCycle=ct.getCyc();
        cout<<"\n\n***SIMULATION ENDED***\n"<<endl<<"Result: Total Utilization Rate: "<<(float)ct.getIns()/totalCycle<<", Total Time: "<<totalCycle<<"cycles."<<endl;
        cout<<"IF UR: "<<(float)accessArr[0]/totalCycle<<", ID UR: "<<(float)accessArr[1]/totalCycle<<", EX UR: "<<(float)accessArr[2]/totalCycle<<", MEM UR: "<<(float)accessArr[3]/totalCycle<<", WB UR: "<<(float)accessArr[4]/totalCycle<<"."<<endl;
        while(1)
        {
            cout<<"\nWould you like to check more information? (Y)es, (N)o"<<endl;
            ch=getchar();
            if(ch=='y'||ch=='Y')
            {
                while(1)
                {
                    cout<<"\nSelect one: (P)ipelined Register, Register (A)rray, (D)ata Memory, (B)ack"<<endl;
                    ch=getchar();
                    if(ch=='P'||ch=='p')
                    {
                        while(1)
                        {
                            cout<<"\nSelect one: I(F)/ID Register, I(D)/EX Register, (E)X/MEM Register, (M)EM/WB Register, (B)ack"<<endl;
                            ch=getchar();
                            if(ch=='F'||ch=='f')
                            {
                                ifidReg.display();
                            }
                            else if(ch=='D'||ch=='d')
                            {
                                idexReg.display();
                            }
                            else if(ch=='E'||ch=='e')
                            {
                                exmemReg.display();
                            }
                            else if(ch=='M'||ch=='m')
                            {
                                memwbReg.display();
                            }
                            else if(ch=='B'||ch=='b')
                            {
                                break;
                            }
                            else
                                cout<<"\nPlease enter the expected letter!"<<endl;
                        }
                    }
                    else if(ch=='A'||ch=='a')
                    {
                        ra.display();
                    }
                    else if(ch=='D'||ch=='d')
                    {
                        dm.display();
                    }
                    else if(ch=='B'||ch=='b')
                    {
                        break;
                    }
                    else
                        cout<<"\nPlease enter the expected letter!"<<endl;
                }
            }
            else if(ch=='N'||ch=='n')
            {
                break;
            }
            else
                cout<<"\nPlease enter the expected letter!"<<endl;
        }
    }
};
    
    
int main()
{
    cout<<"Welcome to MIPS simulator!"<<endl<<endl;
    system("stty -icanon");
    char ch;
    while(1)
    {
        Scheduler scheduler=Scheduler();
        scheduler.run();
        cout<<"\nDo another simulation? (Y)es, (N)o"<<endl;
        cin>>ch;
        if(ch=='N'||ch=='n')
        {
            break;
        }
        else if(ch!='Y'&&ch!='y')
        {
            cout<<"\nPlease enter an expected letter!\n";
        }
    }
    return 0;
}


