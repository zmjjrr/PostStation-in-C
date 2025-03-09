#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"common.h"

extern user* cur_user;
//菜单
void print_menu() {
    printf("***************************************\n");
    printf("\t欢迎使用菜鸟驿站系统\n");
    printf("\t1.登录\n");
    printf("\t2.注册\n");
    printf("\t3.查看包裹\n");
    printf("\t4.进入包裹管理系统\n");
    printf("\t5.保存并退出\n");
    printf("***************************************\n");
    int choice = 0;
    
    scanf("%d", &choice);
    switch (choice) {
    case 1:
        cur_user = login(); 
        break;
    case 2:
        regist(); 
        break;
    case 3:
        inform_user(cur_user); 
        break;
    case 4:
        package_ctrl();
        break;
    case 5:
        destructor();//make sure to call this before exit
        exit(0);
    }
    
}


void inform_user(user* u) {
    if (u == NULL) {
        printf("错误 用户指针为空\n");
        return;
    }
    const char* STATUS[] = { "损坏","正常","丢失" };
    int have_package = 0;
    printf("**用户包裹通知**\n");
    for (int i = 0; i < MAX_INFO_LEN; i++) {
        if (u->packages[i].id == 0)
            break;
        have_package = 1;
        package* p = &(u->packages[i]);
        printf("包裹ID： %d\n", p->id);
        printf("重量：%d\n", p->weight);
        printf("发件人：%s\n", p->sender_name);
        printf("收件人：%s\n", p->receiver_name);
        printf("运输方式：%d\n", p->transport);
        printf("运费：%d元\n", p->cost);
        printf("当前状态：%s\n", STATUS[p->status]);
        printf("==================================\n");
    }
    if (!have_package) {
        printf("您当前没有快递记录\n");
    }
    printf("**********************************\n");
}



