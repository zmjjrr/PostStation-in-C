#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"common.h"

extern package* package_head;
#define Package package
#define next package_fd

// �����°����ڵ�
Package* createPackage(int id, int weight, const char* sender_name, const char* receiver_name,
    int transport, int cost, const char* info, int status) {
    Package* newPackage = (Package*)malloc(sizeof(Package));
    if (newPackage == NULL) {
        printf("�ڴ����ʧ�ܣ�\n");
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

// ��Ӱ���������
void addPackage(Package** head) {
    int id, weight, transport, cost, status;
    char sender_name[MAX_NAME_LEN], receiver_name[MAX_NAME_LEN], info[MAX_INFO_LEN];

    printf("��������� ID: ");
    scanf("%d", &id);
    printf("�������������: ");
    scanf("%d", &weight);
    printf("������ļ�������: ");
    scanf("%s", sender_name);
    printf("�������ռ�������: ");
    scanf("%s", receiver_name);
    printf("���������䷽ʽ: ");
    scanf("%d", &transport);
    printf("�������������: ");
    scanf("%d", &cost);
    printf("�����������Ϣ: ");
    scanf("%s", info);
    printf("���������״̬ (0: ��, 1: ����, 2: ��ʧ): ");
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
    printf("������ӳɹ���\n");
}

// ���� ID ��ѯ����
void queryPackage(Package* head) {
    int id;
    printf("������Ҫ��ѯ�İ��� ID: ");
    scanf("%d", &id);

    Package* current = head;
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
            return;
        }
        current = current->next;
    }
    printf("δ�ҵ��ð�����\n");
}

int package_ctrl() {
    int choice;

    do {
        printf("\n=== ��������ϵͳ ===\n");
        printf("1. ��Ӱ���\n");
        printf("2. ��ѯ����\n");
        printf("3. �˳�\n");
        printf("���������ѡ��: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            addPackage(&package_head);
            break;
        case 2:
            queryPackage(package_head);
            break;
        case 3:
            printf("�˳�ϵͳ��\n");
            break;
        default:
            printf("��Ч��ѡ�����������룡\n");
        }
    } while (choice != 3);

    
    return 0;
}