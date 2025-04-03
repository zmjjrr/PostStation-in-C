#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include"common.h"
#include<time.h>
#pragma comment(lib, "msvcrt.lib")
void record_history(const char* username, const char* action) {
    // 获取当前时间
    time_t current_time;
    struct tm* time_info;
    char time_str[20];

    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);

    // 打开文件以追加模式写入
    FILE* file = fopen("history.txt", "a");
    if (file == NULL) {
        printf("无法打开历史记录文件\n");
        return;
    }

    // 写入记录
    fprintf(file, "%s,%s,%s\n", username, action, time_str);

    // 关闭文件
    fclose(file);
}
 

int is_within_time_range(const char* time_str, const char* range) {
    struct tm time_info;
    time_t time_value;
    time_t current_time;
    struct tm* current_time_info;
    int year, month, day, hour, minute, second;

    // 使用 sscanf 解析时间字符串
    if (sscanf(time_str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) != 6) {
        return 0;
    }

    // 填充 struct tm 结构体
    time_info.tm_year = year - 1900;
    time_info.tm_mon = month - 1;
    time_info.tm_mday = day;
    time_info.tm_hour = hour;
    time_info.tm_min = minute;
    time_info.tm_sec = second;

    // 计算时间戳
    time_value = mktime(&time_info);

    // 获取当前时间
    time(&current_time);
    current_time_info = localtime(&current_time);

    if (strcmp(range, "一小时前") == 0) {
        current_time_info->tm_hour -= 1;
        time_t one_hour_ago = mktime(current_time_info);
        return time_value >= one_hour_ago;
    }
    else if (strcmp(range, "一周前") == 0) {
        current_time_info->tm_mday -= 7;
        time_t one_week_ago = mktime(current_time_info);
        return time_value >= one_week_ago;
    }

    return 0;
}

// 查询历史记录
void query_history(const char* username, const char* action, const char* time_range) {
    FILE* file = fopen("history.txt", "r");
    if (file == NULL) {
        printf("无法打开历史记录文件\n");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        char record_username[20];
        char record_action[50];
        char record_time[20];

        sscanf(line, "%19[^,],%49[^,],%19[^\n]", record_username, record_action, record_time);

        if ((username == NULL || strcmp(username, record_username) == 0) &&
            (action == NULL || strcmp(action, record_action) == 0) &&
            (time_range == NULL || is_within_time_range(record_time, time_range))) {
            printf("用户: %s, 行为: %s, 时间: %s\n", record_username, record_action, record_time);
        }
    }

    fclose(file);
}

void find_history() {
    int choice, action_choice, time_choice;
    char username[20];
    const char* actions[] = { "注册", "登录", "管理用户", "查询用户", "删除用户", "修改密码", "修改电话", "生成", "管理包裹", "添加包裹", "查询包裹", "删除包裹", "寄出包裹", "取件", "入库", "其它" };
    const char* time_ranges[] = { "一小时前", "一周前", "无限制" };
    const char* query_username = NULL;

    while (1) {
        printf("1. 查询历史记录（管理员）\n");
        printf("2. 退出\n");
        scanf("%d", &choice);
        while (getchar() != '\n');

        if (choice == 1) {
            printf("请输入要查询的用户名（留空表示不限制）: ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = '\0';
            if (strcmp(username, "") != 0) {
                query_username = username;
            }

            printf("请选择要查询的行为:\n");
            for (int i = 0; i < 16; i++) {
                printf("%d. %s\n", i + 1, actions[i]);
            }
            printf("选择: ");
            scanf("%d", &action_choice);
            while (getchar() != '\n');
            const char* selected_action = (action_choice >= 1 && action_choice <= 16) ? actions[action_choice - 1] : NULL;

            printf("请选择要查询的时间范围:\n");
            for (int i = 0; i < 3; i++) {
                printf("%d. %s\n", i + 1, time_ranges[i]);
            }
            printf("选择: ");
            scanf("%d", &time_choice);
            while (getchar() != '\n');
            const char* selected_time_range = (time_choice >= 1 && time_choice <= 3) ?
                (strcmp(time_ranges[time_choice - 1], "无限制") == 0 ? NULL : time_ranges[time_choice - 1]) : NULL;

            query_history(query_username, selected_action, selected_time_range);
        }
        else if (choice == 2) {
            break;
        }
        else {
            printf("无效的选择，请重新输入。\n");
        }
    }
}
