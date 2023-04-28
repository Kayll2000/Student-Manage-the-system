#ifndef STUSYSFUN_H
#define STUSYSFUN_H
//#include "stusysfun.h"
#include <vector>
#include <string>
using namespace std;
#define DEBUG     false //测试GM成员函数是否调用成功
#define EXITSYS   0 //退出
#define ADDSTU    1 //增加
#define SHOWSTU   2 //显示
#define DELSTU    3 //删除
#define MODSTU    4 //修改
#define FINDSTU   5 //查询
#define CLEARFILE 6 //清空
#define OTHER     7 //学生数量
#define FILENAME "./StuData/STUFILE.txt"    //保存为TXT文本格式
#define EXCELFILENAME "./StuData/STUInfo.csv"   //保存为Excel表格


class STU   //STU 基类 抽象类
{
public:
    virtual void ShowInfo() = 0;//show 个人信息的虚函数
    
    string stu_name;//姓名
    string stu_id;//学号
    string stu_dept;//职务
};

class NorSTU : public STU   //normal STU 普通学生
{
public:
    NorSTU(string stu_name,string stu_id,string stu_dept);
    void ShowInfo();
};

class  PreSTU : public STU  //PreSTU 班长
{
public:
    PreSTU(string stu_name,string stu_id,string stu_dept);
    void ShowInfo();
};

class  LeaderSTU : public STU   //LeaderSTU 管理员
{
public:
    LeaderSTU(string stu_name,string stu_id,string stu_dept);
    void ShowInfo();
};

class MangerSTU //管理员类
{
public:
    MangerSTU();        //构造函数
    void Show_Menu();       //显示菜单
    void Exit_Sys();        //退出系统
    void Add_Stu();     //增加学生信息
    void Save_Info();       //保存学生信息到文件
    void Init_Stu();        //初始化
    void Show_Stu();        //显示学生信息
    void Del_Stu();     //删除学生信息
    void Mod_Stu();     //修改学生信息
    void Find_Stu();        //查找学生信息
    void Clear_File();      //清空文件
    void Write_to_excel();  //将学生信息保存到Excel表格中
    int StuIsExist(string id);     //判断学号为id的学生信息是否存在，并返回该学生下标
    bool FileIsEmpty;     //标记文件是否为空
    vector<STU*> *Stu_Array; //存放增加的学生信息
    ~MangerSTU();       //析构函数
};


#endif