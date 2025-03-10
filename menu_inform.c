#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"common.h"
// #include"revise.h"

extern user* cur_user;
extern package* package_head;
extern user* user_head;

void default_menu() {
    printf("***************************************\n");
    printf("\t欢迎使用菜鸟驿站系统\n");
    printf("\t1.登录\n");
    printf("\t2.注册\n");
    printf("\t3.保存并退出\n");
    printf("***************************************\n");
}

//用户包裹通知
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
        printf("包裹信息：%s\n", p->info);
        printf("==================================\n");
    }
    if (!have_package) {
        printf("您当前没有快递记录\n");
    }
    printf("**********************************\n");
}

//管理员界面
void admin_menu() {
    printf("***************************************\n");
    printf("\t管理员菜单\n");
    printf("\t1.查看包裹进度\n");
    printf("\t2.包裹管理系统\n");
    printf("\t3.修改个人信息\n");
    printf("\t4.注销\n");//注销
    printf("***************************************\n");
    int choice = 0;
    scanf("%d", &choice);
    switch (choice) {
    case 1:
        // queryPackage(package_head);
        puts("under construction");
        break;
    case 2:
        package_ctrl();
        break;
    case 3:
        // admin_revise(user_head);
        puts("under construction");
        break;
    case 4:
        cur_user = NULL;
        break;
    default:
        printf("无效操作，请重新输入！\n");
    }
}

//用户登录后菜单
void user_menu() {
    printf("***************************************\n");
    printf("\t欢迎用户%s\n", cur_user->username);
    printf("\t1.查看包裹\n");
    printf("\t2.查看用户信息\n");
    printf("\t3.修改用户信息\n");
    printf("\t4.注销\n");
    printf("***************************************\n");
    int choice = 0;
    scanf("%d", &choice);
    switch (choice) {
    case 1:
        inform_user(cur_user);
        break;
    case 2:
        puts("under construction");
        break;
    case 3:
        // user_revise(cur_user);
        puts("under construction");
        break;
    case 4:
        cur_user = NULL;
        break;
    default:
        printf("无效操作，请重新输入！\n");
    }
}
