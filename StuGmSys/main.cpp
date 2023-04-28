/****************************************************************************************************************************
* @function:学生信息管理系统主函数
* @version:v1.0
* @author:chenjunlong
* @description:To achieve a simple student management system.
* @github:https://github.com/Kayll2000/Student-Manage-the-system.git
* @date:2023.04.28
* @lmodauthor:chenjunlong
* @lmoddate:2023.04.28
*           FUCTION:
                    管理员对学生信息的操作：
                        1、录入
                        2、删除
                        3、修改
                        4、查询
                        5、显示学生信息
                        6、清空文件
                        7、退出系统
                        8、······
            BUGFIX:
                    
            MODIFY:

****************************************************************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include<io.h>
#include<direct.h> 
#include "stusysfun.h"
//#include "stisysui.h"

using namespace std;

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
