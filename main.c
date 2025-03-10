#include<stdio.h>
#include<stdlib.h>
#include"common.h"

user* cur_user = NULL;

int main()
{
    constructor();
    while(1)
    {
        default_menu:
        default_menu();
        int choice = 0;
        
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            cur_user = login();
            if (cur_user->privilege == 0) {
                admin:
                admin_menu();
                if(cur_user){
                    goto admin;
                }
                
            }
            else if (cur_user->privilege == 1) {
                user:
                user_menu();
                if(cur_user){
                    goto user;
                }
                
            }
            break;
        case 2:
            regist(); 
            break;
        case 3:
            destructor();//make sure to call this before exit
            exit(0);
        default:
            printf("无效操作，请重新输入！\n");
        }
        

    }
    destructor();
}