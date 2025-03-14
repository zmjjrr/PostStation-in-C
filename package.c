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
// �����°����ڵ�
Package* createPackage(int weight, const char* sender_name, const char* receiver_name,
    int transport, int cost, const char* info, int status) 
{
    Package* newPackage = (Package*)malloc(sizeof(Package));
    if (newPackage == NULL) {
        printf("�ڴ����ʧ�ܣ�\n");
        return NULL;
    }
    // �Զ����������id
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

// ��Ӱ���������
void addPackage(Package** head) {
    int weight, transport, cost, status;
    char sender_name[MAX_NAME_LEN], receiver_name[MAX_NAME_LEN], info[MAX_INFO_LEN];

    printf("������������� (������): ");
    weight = getValidInt(1, 9999); // �������������ΧΪ 1 �� 9999

    printf("������ļ�������: ");
    getValidString(sender_name, MAX_NAME_LEN);

    printf("�������ռ�������: ");
    getValidString(receiver_name, MAX_NAME_LEN);

    printf("���������䷽ʽ (������): ");
    transport = getValidInt(1, 9999); // �������䷽ʽ��ΧΪ 1 �� 9999

    printf("������������� (������): ");
    cost = getValidInt(1, 9999); // ����������÷�ΧΪ 1 �� 9999

    printf("�����������Ϣ: ");
    getValidString(info, MAX_INFO_LEN);

    printf("���������״̬ (0: ��, 1: ����, 2: ��ʧ): ");
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
    printf("������ӳɹ����°���IDΪ��%d\n", newPackage->id);
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
            while (getchar() != '\n'); // ������뻺����
            printf("������Ч��������һ��������\n");
            continue;
        }
        while (getchar() != '\n'); // ������뻺����
        if (num >= min && num <= max) {
            return num;
        }
        printf("���볬����Χ�������� %d �� %d ֮���������\n", min, max);
    }
}
// ɾ����������
void deletePackage(Package **head) {
    if (*head == NULL) {
        printf("û�а�����Ϣ���޷�ɾ����\n");
        return;
    }
    int id;
    printf("������Ҫɾ���İ��� ID: ");
    scanf("%d", &id);
    while (getchar() != '\n');

    Package *current = *head;
    Package *previous = NULL;

    // ����Ҫɾ���İ���
    while (current != NULL && current->id != id) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("δ�ҵ��ð��� ID��ɾ��ʧ�ܣ�\n");
        return;
    }

    // ɾ������
    if (previous == NULL) {
        *head = current->next;
    } else {
        previous->next = current->next;
    }
    free(current);
    reassignIds(*head);
    package_num--;
    printf("����ɾ���ɹ���\n");
}
void queryPackage(Package* head) {
    int choice;
    printf("��ѡ���ѯ��ʽ��\n");
    printf("1. ���ݰ��� ID ��ѯ\n");
    printf("2. ���ݰ���״̬��ѯ\n");
    printf("3. ���ݼļ��˲�ѯ\n");
    printf("4. �����ռ��˲�ѯ\n");
    printf("5. ��ʾ���а�����Ϣ\n");
    scanf("%d", &choice);

    switch (choice) {
        case 1: {
            int id;
            printf("������Ҫ��ѯ�İ��� ID: ");
            scanf("%d", &id);
            Package* current = head;
            int found = 0;
            while (current != NULL) {
                if (current->id == id) {
                    printf("���� ID: %d\n", current->id);
                    printf("����: %d\n", current->weight);
                    printf("�ļ���: %s\n", current->sender_name);
                    printf("�ռ���: %s\n", current->receiver_name);
                    printf("���䷽ʽ: %d\n", current->transport);
                    printf("����: %d\n", current->cost);
                    printf("��Ϣ: %s\n", current->info);
                    printf("״̬: %s\n", current->status == 0 ? "��" : (current->status == 1 ? "����" : "��ʧ"));
                    found = 1;
                    break;
                }
                current = current->next;
            }
            if (!found) {
                printf("δ�ҵ��ð�����\n");
            }
            break;
        }
        case 2: {
            int status;
            printf("������Ҫ��ѯ�İ���״̬ (0: ��, 1: ����, 2: ��ʧ): ");
            scanf("%d", &status);
            Package* current = head;
            int found = 0;
            while (current != NULL) {
                if (current->status == status) {
                    printf("���� ID: %d\n", current->id);
                    printf("����: %d\n", current->weight);
                    printf("�ļ���: %s\n", current->sender_name);
                    printf("�ռ���: %s\n", current->receiver_name);
                    printf("���䷽ʽ: %d\n", current->transport);
                    printf("����: %d\n", current->cost);
                    printf("��Ϣ: %s\n", current->info);
                    printf("״̬: %s\n", current->status == 0 ? "��" : (current->status == 1 ? "����" : "��ʧ"));
                    printf("------------------------\n");
                    found = 1;
                }
                current = current->next;
            }
            if (!found) {
                printf("δ�ҵ����ϸ�״̬�İ�����\n");
            }
            break;
        }
        case 3: {
            char sender[10];
            printf("������Ҫ��ѯ�ļļ�������: ");
            scanf("%s", sender);
            Package* current = head;
            int found = 0;
            while (current != NULL) {
                if (strcmp(current->sender_name, sender) == 0) {
                    printf("���� ID: %d\n", current->id);
                    printf("����: %d\n", current->weight);
                    printf("�ļ���: %s\n", current->sender_name);
                    printf("�ռ���: %s\n", current->receiver_name);
                    printf("���䷽ʽ: %d\n", current->transport);
                    printf("����: %d\n", current->cost);
                    printf("��Ϣ: %s\n", current->info);
                    printf("״̬: %s\n", current->status == 0 ? "��" : (current->status == 1 ? "����" : "��ʧ"));
                    printf("------------------------\n");
                    found = 1;
                }
                current = current->next;
            }
            if (!found) {
                printf("δ�ҵ��üļ��˵İ�����\n");
            }
            break;
        }
        case 4: {
            char receiver[10];
            printf("������Ҫ��ѯ���ռ�������: ");
            scanf("%s", receiver);
            Package* current = head;
            int found = 0;
            while (current != NULL) {
                if (strcmp(current->receiver_name, receiver) == 0) {
                    printf("���� ID: %d\n", current->id);
                    printf("����: %d\n", current->weight);
                    printf("�ļ���: %s\n", current->sender_name);
                    printf("�ռ���: %s\n", current->receiver_name);
                    printf("���䷽ʽ: %d\n", current->transport);
                    printf("����: %d\n", current->cost);
                    printf("��Ϣ: %s\n", current->info);
                    printf("״̬: %s\n", current->status == 0 ? "��" : (current->status == 1 ? "����" : "��ʧ"));
                    printf("------------------------\n");
                    found = 1;
                }
                current = current->next;
            }
            if (!found) {
                printf("δ�ҵ����ռ��˵İ�����\n");
            }
            break;
        }
        case 5: {
            Package* current = head;
            int found = 0;
            while (current != NULL) {
                printf("���� ID: %d\n", current->id);
                printf("����: %d\n", current->weight);
                printf("�ļ���: %s\n", current->sender_name);
                printf("�ռ���: %s\n", current->receiver_name);
                printf("���䷽ʽ: %d\n", current->transport);
                printf("����: %d\n", current->cost);
                printf("��Ϣ: %s\n", current->info);
                printf("״̬: %s\n", current->status == 0 ? "��" : (current->status == 1 ? "����" : "��ʧ"));
                printf("------------------------\n");
                found = 1;
                current = current->next;
            }
            if (!found) {
                printf("û�а�����Ϣ��\n");
            }
            break;
        }
        default:
            printf("��Ч��ѡ�����������룡\n");
            break;
    }
}

int package_ctrl() {
    int choice;

    do {
        printf("\n=== ��������ϵͳ ===\n");
        printf("1. ��Ӱ���\n");
        printf("2. ��ѯ����\n");
        printf("3. ɾ������\n");
        printf("4. �˳�\n");
        printf("���������ѡ��: ");
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
            printf("�˳�ϵͳ��\n");
            break;
        default:
            printf("��Ч��ѡ�����������룡\n");
        }
    } while (choice != 4);

    return 0;
}
