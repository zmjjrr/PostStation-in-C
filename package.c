#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"common.h"

extern package* package_head;



#define Package package
#define next package_fd

static void reassignIds(Package *head) {
    int newId = 1;
    Package *current = head;
    while (current != NULL) {
        current->id = newId;
        newId++;
        current = current->next;
    }
}
// 创建新包裹节点
Package* createPackage(int weight, const char* sender_name, const char* receiver_name,
    int transport, int cost, const char* info, int status) 
{
    Package* newPackage = (Package*)malloc(sizeof(Package));
    if (newPackage == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    // 自动分配递增的id
    newPackage->id = ++package_num;
    newPackage->weight = weight;
    newPackage->transport = transport;
    newPackage->cost = cost;
    newPackage->status = status;
    strncpy(newPackage->sender_name, sender_name, MAX_NAME_LEN - 1);
    newPackage->sender_name[MAX_NAME_LEN - 1] = '\0';
    strncpy(newPackage->receiver_name, receiver_name, MAX_NAME_LEN - 1);
    newPackage->receiver_name[MAX_NAME_LEN - 1] = '\0';
    strncpy(newPackage->info, info, MAX_INFO_LEN - 1);
    newPackage->info[MAX_INFO_LEN - 1] = '\0';
    newPackage->next = NULL;
    return newPackage;
}

// 添加包裹到链表
void addPackage(Package** head) {
    int weight, transport, cost, status;
    char sender_name[MAX_NAME_LEN], receiver_name[MAX_NAME_LEN], info[MAX_INFO_LEN];

    printf("请输入包裹重量 (正整数): ");
    weight = getValidInt(1, 9999); // 假设包裹重量范围为 1 到 9999

    printf("请输入寄件人姓名: ");
    getValidString(sender_name, MAX_NAME_LEN);

    printf("请输入收件人姓名: ");
    getValidString(receiver_name, MAX_NAME_LEN);

    printf("请输入运输方式 (正整数): ");
    transport = getValidInt(1, 9999); // 假设运输方式范围为 1 到 9999

    printf("请输入包裹费用 (正整数): ");
    cost = getValidInt(1, 9999); // 假设包裹费用范围为 1 到 9999

    printf("请输入包裹信息: ");
    getValidString(info, MAX_INFO_LEN);

    printf("请输入包裹状态 (0: 损坏, 1: 正常, 2: 丢失): ");
    status = getValidInt(0, 2);

    Package* newPackage = createPackage(weight, sender_name, receiver_name,
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
    printf("包裹添加成功！新包裹ID为：%d\n", newPackage->id);
}
void getValidString(char* str, int max_len) {
    if (fgets(str, max_len, stdin) != NULL) {
        size_t len = strlen(str);
        if (len > 0 && str[len - 1] == '\n') {
            str[len - 1] = '\0';
        }
    }
}
int getValidInt(int min, int max) {
    int num;
    while (1) {
        if (scanf("%d", &num) != 1) {
            while (getchar() != '\n'); // 清除输入缓冲区
            printf("输入无效，请输入一个整数。\n");
            continue;
        }
        while (getchar() != '\n'); // 清除输入缓冲区
        if (num >= min && num <= max) {
            return num;
        }
        printf("输入超出范围，请输入 %d 到 %d 之间的整数。\n", min, max);
    }
}
// 删除包裹函数
void deletePackage(Package **head) {
    if (*head == NULL) {
        printf("没有包裹信息，无法删除！\n");
        return;
    }
    int id;
    printf("请输入要删除的包裹 ID: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    Package *current = *head;
    Package *previous = NULL;

    // 查找要删除的包裹
    while (current != NULL && current->id != id) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("未找到该包裹 ID，删除失败！\n");
        return;
    }

    // 删除包裹
    if (previous == NULL) {
        *head = current->next;
    } else {
        previous->next = current->next;
    }
    free(current);
    reassignIds(*head);
    package_num--;
    printf("包裹删除成功！\n");
}
void queryPackage(Package* head) {
    int choice;
    printf("请选择查询方式：\n");
    printf("1. 根据包裹 ID 查询\n");
    printf("2. 根据包裹状态查询\n");
    printf("3. 根据寄件人查询\n");
    printf("4. 根据收件人查询\n");
    printf("5. 显示所有包裹信息\n");
    scanf("%d", &choice);

    switch (choice) {
        case 1: {
            int id;
            printf("请输入要查询的包裹 ID: ");
            scanf("%d", &id);
            Package* current = head;
            int found = 0;
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
                    found = 1;
                    break;
                }
                current = current->next;
            }
            if (!found) {
                printf("未找到该包裹！\n");
            }
            break;
        }
        case 2: {
            int status;
            printf("请输入要查询的包裹状态 (0: 损坏, 1: 正常, 2: 丢失): ");
            scanf("%d", &status);
            Package* current = head;
            int found = 0;
            while (current != NULL) {
                if (current->status == status) {
                    printf("包裹 ID: %d\n", current->id);
                    printf("重量: %d\n", current->weight);
                    printf("寄件人: %s\n", current->sender_name);
                    printf("收件人: %s\n", current->receiver_name);
                    printf("运输方式: %d\n", current->transport);
                    printf("费用: %d\n", current->cost);
                    printf("信息: %s\n", current->info);
                    printf("状态: %s\n", current->status == 0 ? "损坏" : (current->status == 1 ? "正常" : "丢失"));
                    printf("------------------------\n");
                    found = 1;
                }
                current = current->next;
            }
            if (!found) {
                printf("未找到符合该状态的包裹！\n");
            }
            break;
        }
        case 3: {
            char sender[10];
            printf("请输入要查询的寄件人姓名: ");
            scanf("%s", sender);
            Package* current = head;
            int found = 0;
            while (current != NULL) {
                if (strcmp(current->sender_name, sender) == 0) {
                    printf("包裹 ID: %d\n", current->id);
                    printf("重量: %d\n", current->weight);
                    printf("寄件人: %s\n", current->sender_name);
                    printf("收件人: %s\n", current->receiver_name);
                    printf("运输方式: %d\n", current->transport);
                    printf("费用: %d\n", current->cost);
                    printf("信息: %s\n", current->info);
                    printf("状态: %s\n", current->status == 0 ? "损坏" : (current->status == 1 ? "正常" : "丢失"));
                    printf("------------------------\n");
                    found = 1;
                }
                current = current->next;
            }
            if (!found) {
                printf("未找到该寄件人的包裹！\n");
            }
            break;
        }
        case 4: {
            char receiver[10];
            printf("请输入要查询的收件人姓名: ");
            scanf("%s", receiver);
            Package* current = head;
            int found = 0;
            while (current != NULL) {
                if (strcmp(current->receiver_name, receiver) == 0) {
                    printf("包裹 ID: %d\n", current->id);
                    printf("重量: %d\n", current->weight);
                    printf("寄件人: %s\n", current->sender_name);
                    printf("收件人: %s\n", current->receiver_name);
                    printf("运输方式: %d\n", current->transport);
                    printf("费用: %d\n", current->cost);
                    printf("信息: %s\n", current->info);
                    printf("状态: %s\n", current->status == 0 ? "损坏" : (current->status == 1 ? "正常" : "丢失"));
                    printf("------------------------\n");
                    found = 1;
                }
                current = current->next;
            }
            if (!found) {
                printf("未找到该收件人的包裹！\n");
            }
            break;
        }
        case 5: {
            Package* current = head;
            int found = 0;
            while (current != NULL) {
                printf("包裹 ID: %d\n", current->id);
                printf("重量: %d\n", current->weight);
                printf("寄件人: %s\n", current->sender_name);
                printf("收件人: %s\n", current->receiver_name);
                printf("运输方式: %d\n", current->transport);
                printf("费用: %d\n", current->cost);
                printf("信息: %s\n", current->info);
                printf("状态: %s\n", current->status == 0 ? "损坏" : (current->status == 1 ? "正常" : "丢失"));
                printf("------------------------\n");
                found = 1;
                current = current->next;
            }
            if (!found) {
                printf("没有包裹信息！\n");
            }
            break;
        }
        default:
            printf("无效的选择，请重新输入！\n");
            break;
    }
}

int package_ctrl() {
    int choice;

    do {
        printf("\n=== 包裹管理系统 ===\n");
        printf("1. 添加包裹\n");
        printf("2. 查询包裹\n");
        printf("3. 删除包裹\n");
        printf("4. 退出\n");
        printf("请输入你的选择: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
        case 1:
            addPackage(&package_head);
            break;
        case 2:
            queryPackage(package_head);
            break;
        case 3:
            deletePackage(&package_head);
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
