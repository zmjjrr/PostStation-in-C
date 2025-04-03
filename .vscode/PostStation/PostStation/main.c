#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include"common.h"

user* cur_user = NULL;

int main()
{
    constructor();
    // 检查货架预警
    title();
    check_shelf_warning();
    int mode = 0;
    while(1)
    {
        if (mode == 0)
        {
            mode = cli();
            continue;
        }
        default_menu();
        int choice = 0;
        choice = getValidInt(1,99);
        switch (choice) {
        case 1:
            cur_user = login();
            if(!cur_user)break;
            record_history(cur_user->username, "登录");
            if (cur_user->privilege == 0) {
                while(cur_user){
                    admin_menu();
                }
            }
            else if (cur_user->privilege == 1) {
                while(cur_user){
                    user_menu();
                }
            }
            else if (cur_user->privilege == 2) {
                while(cur_user){
                    postman_menu();
                } 
            }
            break;
        case 2:
            regist(); 
            save_users();
            break;
        case 3:
            destructor();//make sure to call this before exit
            exit(0);
        case 4:
            mode = 0;
            break;
        default:
            printf("无效操作，请重新输入！\n");
        }
    }
}
