#include<stdio.h>
#include<stdlib.h>
#include"common.h"

user* cur_user = NULL;

int main()
{
    constructor();
    while(1)
    {
        default_menu();
        int choice = 0;
        

        scanf("%d", &choice);
        while (getchar() != '\n');
        switch (choice) {
        case 1:
            cur_user = login();
            if(!cur_user)break;
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
            break;
        case 2:
            regist(); 
            save_users();
            break;
        case 3:
            destructor();//make sure to call this before exit
            exit(0);
        default:
            printf("无效操作，请重新输入！\n");
        }
        

    }
}
