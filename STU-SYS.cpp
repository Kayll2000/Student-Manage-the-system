/**
*************************************************************************
    实现学生信息管理系统
    信息存储：存储在当前目录下的StuData目录下，如果该目录不存在则创建。
        学生类  ——  名字、学号、职务（后续增加联系方式、绩点、地址、宿舍号···）
        学生level：普通学生类   继承于学生类
                   班长类
                   领导者类
        管理员：教务员类
    系统实现，管理员对学生信息的操作：
                            1、录入
                            2、删除
                            3、修改
                            4、查询
                            5、显示学生信息
                            6、清空文件
                            7、退出系统
                            8、······
    后续加入统计学生数量，显示学生信息时
    //2023.3.20 [v2] 1、添加了显示学生信息时，统计和显示系统中学生数量的功能
    				 2、增加了修改信息时，是否保存修改的功能
    				 3、修复系统主菜单在输入非法指令时退出系统的bug，增加输错指令提示并让用户重新输入的功能
    				 4、修复输入的指令不为数字时，系统奔溃的bug
**************************************************************************/
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include<io.h>
#include<direct.h> 

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

MangerSTU::MangerSTU()//管理员构造函数
{
    #if DEBUG
    cout <<"正在构造管理员类···"<< endl;
    #endif

    ifstream fi;
    fi.open(FILENAME,ios::in);  // 允许从流中输入(读取操作)
    if(!fi.is_open())
    {
        cout << "该文件不存在！" << endl;
        this -> FileIsEmpty = true;//文件不存在
        this -> Stu_Array = NULL;//零增加
        fi.close();
        return;
    }
    //文件存在且为空
    char ch;
    fi >> ch;   //读取一个字符
    if(fi.eof())//eof() 函数要跟在读语句以后使用，用来判断前面的读语句是否读到文件结束符EOF了。EOF -- end of file.
    {
        cout <<"该文件为空！"<< endl;
        this -> FileIsEmpty = true;
        this -> Stu_Array = NULL;
        fi.close();
        return;
    }
    //文件存在
    this -> Stu_Array = new vector<STU*>;
    this -> Init_Stu();
}
void MangerSTU::Show_Menu()//显示菜单函数
{
    #if DEBUG
    cout <<"正在显示菜单···"<< endl;
    #endif

    cout <<"-----------------------------------------------------------"<< endl;
    cout <<"---------------欢迎使用”求是“学生管理系统----------------"<< endl;
    cout <<"----------------------0、退出系统--------------------------"<< endl;
    cout <<"---------------------1、增加学生信息-----------------------"<< endl;
    cout <<"---------------------2、显示学生信息-----------------------"<< endl;
    cout <<"---------------------3、删除指定学生信息-------------------"<< endl;
    cout <<"---------------------4、修改指定学生信息-------------------"<< endl; 
    cout <<"---------------------5、查询指定学生信息-------------------"<< endl;
    cout <<"---------------------6、清空文件信息-----------------------"<< endl;
    cout <<"---------------------7、其他功能陆续开发--------------------"<< endl;
    cout <<"-----------------------------------------------------------"<< endl;
    cout << endl;
}
void MangerSTU::Exit_Sys()//退出系统函数
{   
    #if DEBUG
    cout << "正在退出系统···"<< endl;
    #endif

    cout <<"感谢您使用本系统！"<< endl;
    exit(-1); //退出SYSTEM
}

void MangerSTU::Write_to_excel()
{
    // if(_access("StuData", 0) != -1)  
    //     {  
    //         cout << "文件夹已存在" <<endl;  
    //     }  else  {  
    //         cout << "文件夹不存在，创建文件夹" << endl;  
    //         _mkdir("StuData");  
    //     }  
        if(_access("StuData", 0) == -1)  
        {   
            _mkdir("StuData");//创建StuData文件夹，存放系统的学生信息TXT和Excel
        }  

	//cout <<"正在写入Excel"<<endl;
    ofstream opt;
    opt.open(EXCELFILENAME,ios::out|ios::app);//以追加形式打开
    opt <<"姓名"<<","<<"学号"<<","<<"职务"<< endl;
    for(int i=0;i < this -> Stu_Array -> size();++i)
    {
        opt<< this -> Stu_Array -> at(i) -> stu_name <<","
        << this -> Stu_Array -> at(i) -> stu_id <<","
        << this -> Stu_Array -> at(i) -> stu_dept <<endl;
    }
    opt.close();
}

void MangerSTU::Save_Info()//保存信息函数
{
    #if DEBUG
    cout <<"正在保存信息···"<< endl;
    #endif

    if(_access("StuData", 0) == -1)  
    {   
        _mkdir("StuData");//创建文件夹
    }  
    //写入到文件

    ofstream fo;
    /*
    at 用于获得指定字符
    string str;
    cin>>str;  //str=="12345"
    char ch;
    ch = str.at(0);
    cout<<ch;   //ch=='1';
    */
    fo.open(FILENAME,ios::out);//允许输出(写入操作)到流。
    for(int i = 0;i < this -> Stu_Array -> size();++i)
    {
        fo << this -> Stu_Array -> at(i) -> stu_name << " "
        << this -> Stu_Array -> at(i) -> stu_id << " "
        << this -> Stu_Array -> at(i) -> stu_dept << endl;
    }
    fo.close();
}
void MangerSTU::Add_Stu()//增加学生信息函数
{
    #if DEBUG
    cout <<"正在增加学生信息···"<< endl;
    #endif

    if(!this -> Stu_Array)//如果这个学生信息没有存在则执行
    {
        this -> Stu_Array = new vector<STU*>;//开辟空间
        cout <<"开始录入学生信息···"<< endl;
    }
    int i = 1;//记录添加学生
    while(true)
    {
        char flag;
        string _id;
        string _name;
        string _dept;
        //接收用户输入
        cout <<"请输入第"<< i << "个学生的名字"<< endl;
        cin >> _name;
        cout <<"请输入第"<< i << "个学生的学号"<< endl;
        cin >> _id;
        cout <<"请输入第"<< i << "个学生的职务"<< endl;
        cin >> _dept;
        cout <<"职务：[班长，普通学生，领导者] "<< endl;
        //将用户的输入添加到array组中，并开辟各自的空间,创建相应的对象
        STU *_pstu = NULL;//通过对象指针来操作
        if(_dept == "普通学生")
        {
            _pstu = new NorSTU(_name,_id,_dept);
        }else if(_dept == "班长")
        {
            _pstu = new PreSTU(_name,_id,_dept);
        }else if(_dept == "领导者")
        {
            _pstu = new LeaderSTU(_name,_id,_dept);   
        }else
        {
            cout << "该职位不存在!信息录入结束！" << endl;//后续这里改为可以重新输入
            break;
        }
        //将该学生信息存进容器里
        this -> Stu_Array -> push_back(_pstu);
        i++;
        this -> FileIsEmpty = false; //文件非空，更新flag
        cout <<"是否继续录入信息？"<< endl;
        cout <<"Y/y继续录入，N/n结束录入"<< endl;
        while(true){    //非法检测
            cin >> flag;
            if(flag == 'Y' || flag == 'y' || flag == 'N' ||flag == 'n')
            {
                break;
            }else{
                cout <<"输入的指令非法！请重新输入！"<< endl;
            }
        }
        if(flag == 'Y' || flag == 'y')
        {
            continue;
        }else{
            break;
        }
    }
    cout << "成功录入了"<<i-1<<"名学生信息！"<< endl;
    this -> Save_Info();//保存信息
    this -> Write_to_excel();
    system("pause");
    system("cls");
}

void MangerSTU::Init_Stu()//初始化学生信息函数
{
    #if DEBUG
    cout <<"正在初始化学生信息···"<< endl;
    #endif

    ifstream fi;
    fi.open(FILENAME,ios::in);
    string _name;
    string _id;
    string _dept;
    while(fi >> _id && fi >> _name && fi >>_dept)
    {
        STU *_pstu = NULL;
        if(_dept == "班长"){
            _pstu = new PreSTU(_name,_id,_dept);
        }else if(_dept == "领导者"){
            _pstu = new LeaderSTU(_name,_id,_dept);
        }else if(_dept == "普通学生"){
            _pstu = new NorSTU(_name,_id,_dept);
        }
    }
        this -> Save_Info();
        this -> Write_to_excel();
}

void MangerSTU::Show_Stu()//显示学生信息函数
{
    #if DEBUG
    cout <<"正在显示学生信息···"<< endl;
    #endif

    if(this -> FileIsEmpty)//文件为空
    {
        cout << "文件不存在或者文件内容为空！\n请先创建文件并且录入学生信息！"<<endl;
    }
    else{//文件存在而且不为空
        cout <<"系统学生数量为：" << this -> Stu_Array -> size() << endl;
        for(int i = 0;i < this ->Stu_Array -> size();++i)
        {
            this -> Stu_Array -> at(i) ->ShowInfo();
        }
    }
    system("pause");
    system("cls");
}
void MangerSTU::Mod_Stu()//修改学生信息函数
{
    //扩展：可以只要修改某个选项，如只需要修改职务或者只需修改学号和职务···
    #if DEBUG
    cout <<"正在修改学生信息···"<< endl;
    #endif

    if(this -> FileIsEmpty)
    {
        cout << "文件不存在或者文件内容为空！\n请先创建文件并且录入学生信息！"<<endl;
    }
    else{
        cout << "请输入需要修改的学生学号："<< endl;
        string _id;
        cin >> _id;
        int index = StuIsExist(_id);
        if(index != -1)//说明该生信息存在
        {
            string newname;
            string newid;
            string newdept;
			string flag;
            STU *_pstu = NULL;

            cout << "请输入修改后的学生名字："<< endl;
            cin >> newname;

            cout << "请输入修改后的学生学号："<< endl;
            cin >> newid;
            cout << "请输入修改后的学生职务："<< endl;
            cin >> newdept;
            if(newdept == "普通学生")
            {
                _pstu = new NorSTU(newname,newid,newdept);
            }else if(newdept == "班长"){
                _pstu = new PreSTU(newname,newid,newdept);
            }else if(newdept == "领导者"){
                _pstu = new LeaderSTU(newname,newid,newdept);
            }
            cout << "是否保存修改！【Y/N】" << endl;
            while(true){
            cin >> flag;
        	if(flag == "Y" ||flag == "y"||flag == "N" ||flag == "n"){
        		break;
			}else{
				cout << "输入的指令非法，请重新输入！" << endl;
			}
			}
            if(flag == "Y" ||flag == "y"){	
            		this -> Stu_Array -> at(index) = _pstu;
            		cout <<"信息修改成功！"<< endl;
            		this -> Save_Info();
            		this -> Write_to_excel();
            }else{
            		_pstu = NULL;
				}
        }else{
            cout<<"学号不存在，修改操作失败,请重新输入！"<< endl;
            //后续这里改，当输入学号修改，学号不存在时，提示是否需要重新输入学号（Y/N），或者提示是否新增加（Y/N），在此调用add函数      
            }
    }
    system("pause");
}
void MangerSTU::Del_Stu()//删除学生信息函数
{
    #if DEBUG
    cout <<"正在删除学生信息···"<< endl;
    #endif

    if(this -> FileIsEmpty)
    {
        cout << "文件不存在或者文件内容为空！\n请先创建文件并且录入学生信息！"<<endl;
    }else{
        cout << "请输入需要删除的学号："<< endl;
        string _id;
        cin >> _id;
        int index = this -> StuIsExist(_id);
        if(index != -1)
        {
            this -> Stu_Array -> erase(this -> Stu_Array -> begin()+index);
            this -> Save_Info();
            this -> Write_to_excel();
            cout <<"删除成功！"<< endl;
        }else{
            cout <<"该生信息不存在，删除失败！"<< endl;
        }
    }
    system("pause");
}
void MangerSTU::Find_Stu()//查询学生信息函数
{
    #if DEBUG
    cout <<"正在查询学生信息···"<< endl;
    #endif

    if(this -> FileIsEmpty)
    {
        cout << "文件不存在或者文件内容为空！\n请先创建文件并且录入学生信息！"<<endl;
    }else{
        cout <<"输入需要查询的学号："<< endl;
        string _id;
        cin >> _id;
        int index = StuIsExist(_id);
        if(index != -1)
        {
            cout <<"查询成功，该生信息如下："<< endl;
            this -> Stu_Array -> at(index) -> ShowInfo();//显示信息
        }else{
            cout <<"查无此人！"<< endl;
        }
    }
}
void MangerSTU::Clear_File()//清空文件函数
{
    #if DEBUG
    cout <<"正在清空文件···"<< endl;
    #endif

    cout <<"是否清空所有数据？"<< endl;
    cout <<"（Y/y）确定"<< endl;
    cout <<"（N/n）取消"<< endl;
    string select;
    cin >> select;
    if(select == "Y"|| select == "y")
    {
        ofstream fo(FILENAME,ios::trunc);//覆盖存在的文件 即如果原来文件中有数据原来的数据就被清空了权，清空后再写入就可以了
        fo.close();
        if(this -> Stu_Array)//容器非空
        {
            this -> Stu_Array -> clear();//清空容器大小size
            vector<STU*>(*this -> Stu_Array).swap(*this -> Stu_Array);//清空容器容量capacity
            this -> FileIsEmpty = true;
            this -> Stu_Array = NULL;
        }
        cout <<"清空成功！"<< endl;
    }
    system("pause");
    system("cls");
}
int MangerSTU::StuIsExist(string id)//判断该生信息是否存在
{
    #if DEBUG
    cout <<"正在判断该生信息是否存在···"<< endl;
    #endif
    int _len = this -> Stu_Array -> size();//获取学生信息容器存放的数量
    int index = -1;
    for(int i = 0; i < _len; ++i)//遍历容器
    {
        if(this -> Stu_Array -> at(i) -> stu_id == id)
        {
            index = i;
            break;
        }
    }
    return index;//index = -1说明该学号不存在
}

MangerSTU::~MangerSTU()//析构函数实现
{
    #if DEBUG
    cout <<"正在析构管理员类···"<< endl;
    #endif

    if(this -> Stu_Array)//容器非空
    {
        this -> Stu_Array -> clear();
        delete[] this -> Stu_Array;
        this -> Stu_Array = NULL;//删除后，让对象指针指向空，防止悬空
    }
}
NorSTU::NorSTU(string name,string id,string dept)//普通学生构造函数
{
    #if DEBUG
    cout <<"正在构造普通学生类···"<< endl;
    #endif
    this -> stu_name = name;
    this -> stu_id = id;
    this -> stu_dept = dept;
}
void NorSTU::ShowInfo()//普通学生成员函数实现
{
    #if DEBUG
    cout <<"正在显示普通学生信息···"<< endl;
    #endif

    cout <<"学生名字："<<this -> stu_name<<"\t学生学号："
    <<this -> stu_id<<"\t学生职务："
    <<this -> stu_dept<<"任务：遵守纪律，服从安排！"<<endl;
}
PreSTU::PreSTU(string name,string id,string dept)//班长构造函数
{
    #if DEBUG
    cout <<"正在构造班长类···"<< endl;
    #endif

    this -> stu_name = name;
    this -> stu_id = id;
    this -> stu_dept = dept;
    
}
void PreSTU::ShowInfo()//班长成员函数实现
{
    #if DEBUG
    cout<<"正在显示班长信息···"<< endl;
    #endif

    cout <<"学生名字："<<this -> stu_name<<"\t学生学号："
    <<this -> stu_id<<"\t学生职务："
    <<this -> stu_dept<<"任务：管理班级，对接辅导员！"<<endl;
}

LeaderSTU::LeaderSTU(string name,string id,string dept)//领导者构造函数
{
    #if DEBUG
    cout<<"正在构造领导者类···"<< endl;
    #endif

    this -> stu_name = name;
    this -> stu_id = id;
    this -> stu_dept = dept;
}
void LeaderSTU::ShowInfo()//领导者函数实现
{
    #if DEBUG
    cout <<"正在显示领导者信息···"<< endl;
    #endif

    cout <<"学生名字："<<this -> stu_name<<"\t学生学号："
    <<this -> stu_id<<"\t学生职务："
    <<this -> stu_dept<<"任务：领导班级，管理人员！"<<endl;
}

int main()
{
    MangerSTU gm;   //实例化管理员
    int select;     //存储用户选项

    while(true)
    {
        gm.Show_Menu();
        cout <<"请输入需要操作的指令："<< endl;
		while(true){
			cin >> select;
  			if(!cin)
  			{
  				cout << "error" << endl;
     			cin.clear();
     			select = 1024;
    			cin.get();
    			//cout <<"debug:" << select << endl;
     		}
			if(select >= EXITSYS && select <= OTHER)
			{
				break;
			}else{
				cout << "输入指令非法，请重新输入！" << endl;
				system("cls");  //清屏操作
				gm.Show_Menu();
				cout <<"请输入需要操作的指令："<< endl;
			}
		}
        switch(select)
        {    case EXITSYS:
                gm.Exit_Sys();
                break;
            case ADDSTU:
                gm.Add_Stu();
                break;
            case SHOWSTU:
                gm.Show_Stu();
                break;
            case DELSTU:
                gm.Del_Stu();
                break;
            case MODSTU:
                gm.Mod_Stu();
                break;
            case FINDSTU:
                gm.Find_Stu();
                break;
            case CLEARFILE:
                gm.Clear_File();
                break;
            case OTHER:
                cout << "功能未开放！" << endl;
                break;
            default:
                system("cls");  //清屏操作
                break;
        }
    }

    system("pause");
    return 0;
}
