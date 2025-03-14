#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"common.h"

extern package* package_head;
#define Package package
#define next package_fd

// 创建新包裹节点
Package* createPackage(int id, int weight, const char* sender_name, const char* receiver_name,
    int transport, int cost, const char* info, int status) {
    Package* newPackage = (Package*)malloc(sizeof(Package));
    if (newPackage == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    newPackage->id = id;
    newPackage->weight = weight;
    strncpy(newPackage->sender_name, sender_name, MAX_NAME_LEN - 1);
    newPackage->sender_name[MAX_NAME_LEN - 1] = '\0';
    strncpy(newPackage->receiver_name, receiver_name, MAX_NAME_LEN - 1);
    newPackage->receiver_name[MAX_NAME_LEN - 1] = '\0';
    newPackage->transport = transport;
    newPackage->cost = cost;
    strncpy(newPackage->info, info, MAX_INFO_LEN - 1);
    newPackage->info[MAX_INFO_LEN - 1] = '\0';
    newPackage->status = status;
    newPackage->next = NULL;
    return newPackage;
}

// 添加包裹到链表
void addPackage(Package** head) {
    int id, weight, transport, cost, status;
    char sender_name[MAX_NAME_LEN], receiver_name[MAX_NAME_LEN], info[MAX_INFO_LEN];

    printf("请输入包裹 ID: ");
    scanf("%d", &id);
    printf("请输入包裹重量: ");
    scanf("%d", &weight);
    printf("请输入寄件人姓名: ");
    scanf("%s", sender_name);
    printf("请输入收件人姓名: ");
    scanf("%s", receiver_name);
    printf("请输入运输方式: ");
    scanf("%d", &transport);
    printf("请输入包裹费用: ");
    scanf("%d", &cost);
    printf("请输入包裹信息: ");
    scanf("%s", info);
    printf("请输入包裹状态 (0: 损坏, 1: 正常, 2: 丢失): ");
    scanf("%d", &status);

    Package* newPackage = createPackage(id, weight, sender_name, receiver_name,
        transport, cost, info, status);
    if (newPackage == NULL) {
        return;
    }

    if (*head == NULL) {
        *head = newPackage;
    }
    else {
        Package* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newPackage;
    }
    printf("包裹添加成功！\n");
}

// 根据 ID 查询包裹
void queryPackage(Package* head) {
    int id;
    printf("请输入要查询的包裹 ID: ");
    scanf("%d", &id);

    Package* current = head;
    while (current != NULL) {
        if (current->id == id) {
            printf("包裹 ID: %d\n", current->id);
            printf("重量: %d\n", current->weight);
            printf("寄件人: %s\n", current->sender_name);
            printf("收件人: %s\n", current->receiver_name);
            printf("运输方式: %d\n", current->transport);
            printf("费用: %d\n", current->cost);
            printf("信息: %s\n", current->info);
            printf("状态: %s\n", current->status == 0 ? "损坏" : (current->status == 1 ? "正常" : "丢失"));
            return;
        }
        current = current->next;
    }
    printf("未找到该包裹！\n");
}

int package_ctrl() {
    int choice;

    do {
        printf("\n=== 包裹管理系统 ===\n");
        printf("1. 添加包裹\n");
        printf("2. 查询包裹\n");
        printf("3. 退出\n");
        printf("请输入你的选择: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            addPackage(&package_head);
            break;
        case 2:
            queryPackage(package_head);
            break;
        case 3:
            printf("退出系统！\n");
            break;
        default:
            printf("无效的选择，请重新输入！\n");
        }
    } while (choice != 3);

    
    return 0;
}