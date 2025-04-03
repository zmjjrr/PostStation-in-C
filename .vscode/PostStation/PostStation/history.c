#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include"common.h"
#include<time.h>
#pragma comment(lib, "msvcrt.lib")
void record_history(const char* username, const char* action) {
    // ��ȡ��ǰʱ��
    time_t current_time;
    struct tm* time_info;
    char time_str[20];

    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);

    // ���ļ���׷��ģʽд��
    FILE* file = fopen("history.txt", "a");
    if (file == NULL) {
        printf("�޷�����ʷ��¼�ļ�\n");
        return;
    }

    // д���¼
    fprintf(file, "%s,%s,%s\n", username, action, time_str);

    // �ر��ļ�
    fclose(file);
}
 

int is_within_time_range(const char* time_str, const char* range) {
    struct tm time_info;
    time_t time_value;
    time_t current_time;
    struct tm* current_time_info;
    int year, month, day, hour, minute, second;

    // ʹ�� sscanf ����ʱ���ַ���
    if (sscanf(time_str, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) != 6) {
        return 0;
    }

    // ��� struct tm �ṹ��
    time_info.tm_year = year - 1900;
    time_info.tm_mon = month - 1;
    time_info.tm_mday = day;
    time_info.tm_hour = hour;
    time_info.tm_min = minute;
    time_info.tm_sec = second;

    // ����ʱ���
    time_value = mktime(&time_info);

    // ��ȡ��ǰʱ��
    time(&current_time);
    current_time_info = localtime(&current_time);

    if (strcmp(range, "һСʱǰ") == 0) {
        current_time_info->tm_hour -= 1;
        time_t one_hour_ago = mktime(current_time_info);
        return time_value >= one_hour_ago;
    }
    else if (strcmp(range, "һ��ǰ") == 0) {
        current_time_info->tm_mday -= 7;
        time_t one_week_ago = mktime(current_time_info);
        return time_value >= one_week_ago;
    }

    return 0;
}

// ��ѯ��ʷ��¼
void query_history(const char* username, const char* action, const char* time_range) {
    FILE* file = fopen("history.txt", "r");
    if (file == NULL) {
        printf("�޷�����ʷ��¼�ļ�\n");
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
            printf("�û�: %s, ��Ϊ: %s, ʱ��: %s\n", record_username, record_action, record_time);
        }
    }

    fclose(file);
}

void find_history() {
    int choice, action_choice, time_choice;
    char username[20];
    const char* actions[] = { "ע��", "��¼", "�����û�", "��ѯ�û�", "ɾ���û�", "�޸�����", "�޸ĵ绰", "����", "�������", "��Ӱ���", "��ѯ����", "ɾ������", "�ĳ�����", "ȡ��", "���", "����" };
    const char* time_ranges[] = { "һСʱǰ", "һ��ǰ", "������" };
    const char* query_username = NULL;

    while (1) {
        printf("1. ��ѯ��ʷ��¼������Ա��\n");
        printf("2. �˳�\n");
        scanf("%d", &choice);
        while (getchar() != '\n');

        if (choice == 1) {
            printf("������Ҫ��ѯ���û��������ձ�ʾ�����ƣ�: ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = '\0';
            if (strcmp(username, "") != 0) {
                query_username = username;
            }

            printf("��ѡ��Ҫ��ѯ����Ϊ:\n");
            for (int i = 0; i < 16; i++) {
                printf("%d. %s\n", i + 1, actions[i]);
            }
            printf("ѡ��: ");
            scanf("%d", &action_choice);
            while (getchar() != '\n');
            const char* selected_action = (action_choice >= 1 && action_choice <= 16) ? actions[action_choice - 1] : NULL;

            printf("��ѡ��Ҫ��ѯ��ʱ�䷶Χ:\n");
            for (int i = 0; i < 3; i++) {
                printf("%d. %s\n", i + 1, time_ranges[i]);
            }
            printf("ѡ��: ");
            scanf("%d", &time_choice);
            while (getchar() != '\n');
            const char* selected_time_range = (time_choice >= 1 && time_choice <= 3) ?
                (strcmp(time_ranges[time_choice - 1], "������") == 0 ? NULL : time_ranges[time_choice - 1]) : NULL;

            query_history(query_username, selected_action, selected_time_range);
        }
        else if (choice == 2) {
            break;
        }
        else {
            printf("��Ч��ѡ�����������롣\n");
        }
    }
}
