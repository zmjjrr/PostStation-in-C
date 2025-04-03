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
    int transport, int cost, const char* info, int type, int status, int T_status,double volume,double distance) 
{ 
    Package* newPackage = (Package*)calloc(1,sizeof(Package));
    if (newPackage == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }
    // 自动分配递增的id
    newPackage->id = ++package_num;
    newPackage->weight = weight;
    newPackage->transport = transport;
    newPackage->cost = cost;
    newPackage->type = type;
    newPackage->distance = distance;
    newPackage->volume = volume;
    newPackage->status = status;
    if (cur_user->privilege == 2)
    {
        newPackage->T_status = 0;
    }
    else if(cur_user->privilege == 1)
    {
        newPackage->T_status = 1;
    }
    
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
void addPackage(package** head) {
    int  transport,  status,type;
    double volume, distance,cost,weight;
    char sender_name[MAX_NAME_LEN], receiver_name[MAX_NAME_LEN], info[MAX_INFO_LEN];

    printf("请输入包裹重量 (精确到g): ");
    scanf("%lf", &weight); // 假设包裹重量范围为 1 到 9999
    while (getchar() != '\n');
    printf("请输入包裹体积（精确到立方米）:");
    scanf("%lf", &volume);
    while (getchar() != '\n');
    printf("请输入包裹运输距离（精确到米）:");
    scanf("%lf", &distance);
    while (getchar()!='\n');

    printf("请输入寄件人姓名: ");
    getValidString(sender_name, MAX_NAME_LEN);

    printf("请输入收件人姓名: ");
    getValidString(receiver_name, MAX_NAME_LEN);

    printf("请输入包裹类型 (0：普通快递，1：生鲜快递，2：贵重快递，3：大件快递): ");
    type = getValidInt(0, 3); 

    printf("请输入运输方式 (1：空运，2：公路，3：铁路，4：骑手，5：无人机): ");
    transport = getValidInt(1, 5); 

    printf("请输入包裹信息: ");
    getValidString(info, MAX_INFO_LEN);

    printf("请输入包裹状态 (0: 损坏, 1: 正常, 2: 丢失): ");
    status = getValidInt(0, 2);

    cost = 0;

    Package* newPackage = createPackage(weight, sender_name, receiver_name,
                                        transport, cost,info, type , status, 0,volume,distance);
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
    newPackage->cost = calc_cost(newPackage,cur_user->level);   
    printf("包裹添加成功！新包裹ID为：%d\n", newPackage->id);
    printf("包裹费用为：%.2f\n",newPackage->cost );
      

      // 更新用户累计消费金额
    if (cur_user->privilege == 2)
    {
        cur_user->total_cost += calc_cost(newPackage, cur_user->level);
        update_user_level(cur_user);
    }
     
}
void getValidString(char* str, int max_len) {
    fgets(str, max_len, stdin);
    str[strcspn(str, "\n")] = '\0';
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
    id = getValidInt(1, 9999); // 假设包裹 ID 范围为 1 到 9999

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
void printPackageInfo(package *current)
{
    char shelf[] = "ABCD";
    printf("包裹 ID: %c%03d\n", shelf[current->type] ,current->id);
    printf("重量: %.2f\n", current->weight);
    printf("寄件人: %s\n", current->sender_name);
    printf("收件人: %s\n", current->receiver_name);
    printf("运输方式: %d\n", current->transport);
    printf("费用: %.2f\n", current->cost);
    printf("信息: %s\n", current->info);
    printf("类型: %s\n", current->type == 0? "普通快递" : (current->type == 1? "生鲜快递" : (current->type == 2? "贵重快递" : "大件快递")));
    printf("状态: %s\n", current->status == 0 ? "损坏" : (current->status == 1 ? "正常" : "丢失"));
    printf("运输状态: %s\n", current->T_status == 0? "待寄出" : (current->T_status == 1? "待取出" : "运输中"));
}
int queryPackageMenu()
{
    int choice;
    printf("请选择查询方式：\n");
    printf("1. 根据包裹 ID 查询\n");
    printf("2. 根据包裹状态查询\n");
    printf("3. 根据寄件人查询\n");
    printf("4. 根据收件人查询\n");
    printf("5. 显示所有包裹信息\n");
    choice = getValidInt(1, 5);
    return choice;
}
void queryPackage(Package* head,int choice) 
{
    switch (choice) {
        case 1: {
            int id;
            printf("请输入要查询的包裹 ID: ");
            id = getValidInt(1, 9999); // 假设包裹 ID 范围为 1 到 9999
            Package* current = head;
            int found = 0;
            while (current != NULL) {
                if (current->id == id) {
                    printPackageInfo(current);
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
            status = getValidInt(0, 2);
            Package* current = head;
            int found = 0;
            while (current != NULL) {
                if (current->status == status) {
                    printPackageInfo(current);
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
            getValidString(sender, MAX_NAME_LEN);
            Package* current = head;
            int found = 0;
            while (current != NULL) {
                if (strcmp(current->sender_name, sender) == 0) {
                    printPackageInfo(current);
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
            char receiver[30];
            printf("请输入要查询的收件人姓名: ");
            getValidString(receiver, MAX_NAME_LEN);
            Package* current = head;
            int found = 0;
            while (current != NULL) {
                if (strcmp(current->receiver_name, receiver) == 0) {
                    printPackageInfo(current);
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
                printPackageInfo(current);
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
        choice = getValidInt(1, 4);

        switch (choice) {
        case 1:
            addPackage(&package_head);
            save_packages();
            record_history(cur_user->username, "添加包裹");
            break;
        case 2:
            queryPackage(package_head,queryPackageMenu());
            record_history(cur_user->username, "查询包裹");
            break;
        case 3:
            deletePackage(&package_head);
            save_packages();
            record_history(cur_user->username, "删除包裹");
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

// 修改包裹信息的函数
void modify_package_info(package *pkg, int attribute, int int_value, const char *str_value) {
    switch (attribute) {
        case 1:
            pkg->weight = int_value;
            break;
        case 2:
            pkg->transport = int_value;
            break;
        case 3:
            pkg->cost = int_value;
            break;
        case 4:
            pkg->status = int_value;
            break;
        case 5:
            pkg->T_status = int_value;
            break;
        case 6:
            strncpy(pkg->sender_name, str_value, MAX_NAME_LEN - 1);
            pkg->sender_name[MAX_NAME_LEN - 1] = '\0';
            break;
        case 7:
            strncpy(pkg->receiver_name, str_value, MAX_NAME_LEN - 1);
            pkg->receiver_name[MAX_NAME_LEN - 1] = '\0';
            break;
        case 8:
            strncpy(pkg->info, str_value, MAX_INFO_LEN - 1);
            pkg->info[MAX_INFO_LEN - 1] = '\0';
            break;
        default:
            printf("无效的属性选择，请重新输入。\n");
            return;
    }
    printf("包裹信息修改成功。\n");
}

void pick_package(package* p)
{
    if (p->T_status == 0)
    {
        p->T_status = 2;
        printf("已取出ID为%d的包裹\n", p->id);
    }
    else
    {
        printf("取件失败，请查看包裹状态\n");
    }

}

void store_package()
{
    addPackage(&package_head);
    printf("入库成功！\n");
}

double calc_cost(package *pkg,int user_level)
{
    double baseCost = 0;
    double weightFactor = 0;
    double volumeFactor = 0;
    double distanceFactor = 0;

    // 根据包裹类型设置基础费用和费用因子
    switch (pkg->type) {
    case 0:  // 普通快递
        baseCost = 10;
        weightFactor = 1;
        volumeFactor = 0.5;
        break;
    case 1:  // 生鲜快递
        baseCost = 20;
        weightFactor = 1.5;
        volumeFactor = 1;
        break;
    case 2:  // 贵重快递
        baseCost = 30;
        weightFactor = 2;
        volumeFactor = 1.5;
        break;
    case 3:  // 大件快递
        baseCost = 40;
        weightFactor = 2.5;
        volumeFactor = 2;
        break;
    default:
        return -1;  // 无效的包裹类型
    }

    // 根据运输方式调整费用
    switch (pkg->transport) {
   
    case 1:  // 空运
        distanceFactor = 0.5;
        break;
    case 2:  // 公路
       
        distanceFactor = 0.2;
        break;
    case 3:  // 铁路
        
        distanceFactor = 0.3;
        break;
    case 4:  // 骑手
        
        distanceFactor = 0.1;
        break;
    case 5:  // 无人机
        
        distanceFactor = 0.4;
        break;

    default:
        return -1;  // 无效的运输方式
    }

    // 计算重量、体积和距离的费用
    double weightCost = pkg->weight * weightFactor;
    double volumeCost = pkg->volume * volumeFactor;
    double distanceCost = pkg->distance * distanceFactor;

    // 计算总费用
    double totalCost = baseCost + weightCost + volumeCost + distanceCost;
    double discountFactor[5] = { 0.9,0.8,0.7,0.6,0.5 };
    totalCost = totalCost * discountFactor[user_level];
    return totalCost;
}
// 检查货架预警函数实现
void check_shelf_warning() {
    for (int i = 0; i < 4; i++) {
        int count = 0;
        for (int j = 0; j < MAX_PACAKGE_NUM; j++) {
            if (Shelves[i][j] != NULL) {
                count++;
            }
        }
        double usage = (double)count / MAX_PACAKGE_NUM;
        if (usage >= SHELF_WARNING_THRESHOLD) {
            printf("警告：货架 %c 已使用 %.2f%%，接近满负荷！\n", 'A' + i, usage * 100);
        }
    }
}