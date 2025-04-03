#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"common.h"


void default_menu() {
    printf("***************************************\n");
    printf("\t欢迎使用菜鸟驿站系统\n");
    printf("\t1.登录\n");
    printf("\t2.注册\n");
    printf("\t3.保存并退出\n");
    printf("\t4.进入命令行界面\n");
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
        printPackageInfo(p);
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
    printf("\t1.管理包裹\n");
    printf("\t2.管理用户\n");
    printf("\t3.查询历史记录\n");
    printf("\t4.生成邀请码\n");//注销
    printf("\t5.注销\n");
    printf("***************************************\n");
    int choice = 0;
    char keystr[33];
    choice = getValidInt(1, 5);
    switch (choice) {
    case 1:
        package_ctrl();
        record_history(cur_user->username, "管理包裹");
        break;
    case 2:
        user_ctrl();
        save_users();
        record_history(cur_user->username, "管理用户");
        break;
    case 3:
        find_history();
        break;
    case 4:
        keygen(keystr);//pass a pointer to the char array
        puts(keystr);
        record_history(cur_user->username, "生成邀请码");
        break;
    case 5:
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
    printf("\t2 寄出包裹\n");
    printf("\t3.查看用户信息\n");
    printf("\t4.修改用户信息\n");
    printf("\t5.注销\n");
    printf("***************************************\n");
    int choice = 0;
    choice = getValidInt(1, 5);
    switch (choice) {
    case 1:
        inform_user(cur_user);
        record_history(cur_user->username, "查询包裹");
        break;
    case 2:
        addPackage(&package_head);
        record_history(cur_user->username, "寄出包裹");
        save_packages();
        break;
    case 3:
        printUserInfo(cur_user);
        break;
    case 4:
        user_revise(cur_user, user_head);
        record_history(cur_user->username, "修改信息");
        save_users();
        break;
    case 5:
        cur_user = NULL;
        break;
    default:
        printf("无效操作，请重新输入！\n");
    }
}
             
int user_ctrl() {
    int choice;

    do {
        printf("\n=== 用户管理系统 ===\n");
        printf("1. 查询用户\n");
        printf("2. 删除用户\n");
        printf("3. 修改用户信息\n");
        printf("4. 退出\n");
        printf("请输入你的选择: ");
        choice = getValidInt(1, 99);

        switch (choice) {
        case 1:
            queryUser(user_head);
            record_history(cur_user->username, "查询用户");
            break;
        case 2:
            deleteUser(&user_head);
            record_history(cur_user->username, "删除用户");
            break;
        case 3:
            admin_revise(user_head);
            record_history(cur_user->username, "修改信息");
            break;
        case 4:
            printf("退出系统！\n");
            break;
        default:
            printf("无效的选择，请重新输入！\n");
        }
    } while (choice != 4);

    return 0;
}

// 快递员界面
void postman_menu() {
    printf("***************************************\n");
    printf("\t快递员菜单\n");
    printf("\t1. 取件\n");
    printf("\t2. 入库\n");
    printf("\t3. 查询\n");
    printf("\t4. 查看用户信息\n");
    printf("\t5. 修改用户信息\n");
    printf("\t6. 退出登录\n");
    printf("***************************************\n");

    int choice = 0;
    int found = 0;
    int id = 0;
    choice = getValidInt(1, 6);
    char sender[MAX_NAME_LEN];
    package* current = package_head;
    strcpy(sender, cur_user->username);

    switch (choice) {
    case 1:
        printf("请输入包裹id:");
        id = getValidInt(1, package_num);
        package* p = package_head;
        while (p != NULL)
        {
            if (p->id == id)
            {
                pick_package(p);
                break;
            }
            p = p->package_fd;
        }
        if (p == NULL)
        {
            printf("未找到该包裹\n");
        }
        save_packages();
        record_history(cur_user->username, "取件");
        break;
    case 2:
        store_package();
        save_packages();
        record_history(cur_user->username, "入库");
        break;
    case 3:
        queryPackage(package_head,queryPackageMenu());
        record_history(cur_user->username, "查询包裹");
        break;
    case 4:
        printUserInfo(cur_user);
        record_history(cur_user->username, "查询用户");
        break;
    case 5:
        user_revise(cur_user,user_head);
        break;
    case 6:
        cur_user = NULL;
        break;
    default:
        printf("无效操作，请重新输入！\n");
    }
}