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
    int transport, int cost, const char* info, int type, int status, int T_status,double volume,double distance) 
{ 
    Package* newPackage = (Package*)calloc(1,sizeof(Package));
    if (newPackage == NULL) {
        printf("�ڴ����ʧ�ܣ�\n");
        return NULL;
    }
    // �Զ����������id
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

// ��Ӱ���������
void addPackage(package** head) {
    int  transport,  status,type;
    double volume, distance,cost,weight;
    char sender_name[MAX_NAME_LEN], receiver_name[MAX_NAME_LEN], info[MAX_INFO_LEN];

    printf("������������� (��ȷ��g): ");
    scanf("%lf", &weight); // �������������ΧΪ 1 �� 9999
    while (getchar() != '\n');
    printf("����������������ȷ�������ף�:");
    scanf("%lf", &volume);
    while (getchar() != '\n');
    printf("���������������루��ȷ���ף�:");
    scanf("%lf", &distance);
    while (getchar()!='\n');

    printf("������ļ�������: ");
    getValidString(sender_name, MAX_NAME_LEN);

    printf("�������ռ�������: ");
    getValidString(receiver_name, MAX_NAME_LEN);

    printf("������������� (0����ͨ��ݣ�1�����ʿ�ݣ�2�����ؿ�ݣ�3��������): ");
    type = getValidInt(0, 3); 

    printf("���������䷽ʽ (1�����ˣ�2����·��3����·��4�����֣�5�����˻�): ");
    transport = getValidInt(1, 5); 

    printf("�����������Ϣ: ");
    getValidString(info, MAX_INFO_LEN);

    printf("���������״̬ (0: ��, 1: ����, 2: ��ʧ): ");
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
    printf("������ӳɹ����°���IDΪ��%d\n", newPackage->id);
    printf("��������Ϊ��%.2f\n",newPackage->cost );
      

      // �����û��ۼ����ѽ��
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
    id = getValidInt(1, 9999); // ������� ID ��ΧΪ 1 �� 9999

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
void printPackageInfo(package *current)
{
    char shelf[] = "ABCD";
    printf("���� ID: %c%03d\n", shelf[current->type] ,current->id);
    printf("����: %.2f\n", current->weight);
    printf("�ļ���: %s\n", current->sender_name);
    printf("�ռ���: %s\n", current->receiver_name);
    printf("���䷽ʽ: %d\n", current->transport);
    printf("����: %.2f\n", current->cost);
    printf("��Ϣ: %s\n", current->info);
    printf("����: %s\n", current->type == 0? "��ͨ���" : (current->type == 1? "���ʿ��" : (current->type == 2? "���ؿ��" : "������")));
    printf("״̬: %s\n", current->status == 0 ? "��" : (current->status == 1 ? "����" : "��ʧ"));
    printf("����״̬: %s\n", current->T_status == 0? "���ĳ�" : (current->T_status == 1? "��ȡ��" : "������"));
}
int queryPackageMenu()
{
    int choice;
    printf("��ѡ���ѯ��ʽ��\n");
    printf("1. ���ݰ��� ID ��ѯ\n");
    printf("2. ���ݰ���״̬��ѯ\n");
    printf("3. ���ݼļ��˲�ѯ\n");
    printf("4. �����ռ��˲�ѯ\n");
    printf("5. ��ʾ���а�����Ϣ\n");
    choice = getValidInt(1, 5);
    return choice;
}
void queryPackage(Package* head,int choice) 
{
    switch (choice) {
        case 1: {
            int id;
            printf("������Ҫ��ѯ�İ��� ID: ");
            id = getValidInt(1, 9999); // ������� ID ��ΧΪ 1 �� 9999
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
                printf("δ�ҵ��ð�����\n");
            }
            break;
        }
        case 2: {
            int status;
            printf("������Ҫ��ѯ�İ���״̬ (0: ��, 1: ����, 2: ��ʧ): ");
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
                printf("δ�ҵ����ϸ�״̬�İ�����\n");
            }
            break;
        }
        case 3: {
            char sender[10];
            printf("������Ҫ��ѯ�ļļ�������: ");
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
                printf("δ�ҵ��üļ��˵İ�����\n");
            }
            break;
        }
        case 4: {
            char receiver[30];
            printf("������Ҫ��ѯ���ռ�������: ");
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
                printf("δ�ҵ����ռ��˵İ�����\n");
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
        choice = getValidInt(1, 4);

        switch (choice) {
        case 1:
            addPackage(&package_head);
            save_packages();
            record_history(cur_user->username, "��Ӱ���");
            break;
        case 2:
            queryPackage(package_head,queryPackageMenu());
            record_history(cur_user->username, "��ѯ����");
            break;
        case 3:
            deletePackage(&package_head);
            save_packages();
            record_history(cur_user->username, "ɾ������");
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

// �޸İ�����Ϣ�ĺ���
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
            printf("��Ч������ѡ�����������롣\n");
            return;
    }
    printf("������Ϣ�޸ĳɹ���\n");
}

void pick_package(package* p)
{
    if (p->T_status == 0)
    {
        p->T_status = 2;
        printf("��ȡ��IDΪ%d�İ���\n", p->id);
    }
    else
    {
        printf("ȡ��ʧ�ܣ���鿴����״̬\n");
    }

}

void store_package()
{
    addPackage(&package_head);
    printf("���ɹ���\n");
}

double calc_cost(package *pkg,int user_level)
{
    double baseCost = 0;
    double weightFactor = 0;
    double volumeFactor = 0;
    double distanceFactor = 0;

    // ���ݰ����������û������úͷ�������
    switch (pkg->type) {
    case 0:  // ��ͨ���
        baseCost = 10;
        weightFactor = 1;
        volumeFactor = 0.5;
        break;
    case 1:  // ���ʿ��
        baseCost = 20;
        weightFactor = 1.5;
        volumeFactor = 1;
        break;
    case 2:  // ���ؿ��
        baseCost = 30;
        weightFactor = 2;
        volumeFactor = 1.5;
        break;
    case 3:  // ������
        baseCost = 40;
        weightFactor = 2.5;
        volumeFactor = 2;
        break;
    default:
        return -1;  // ��Ч�İ�������
    }

    // �������䷽ʽ��������
    switch (pkg->transport) {
   
    case 1:  // ����
        distanceFactor = 0.5;
        break;
    case 2:  // ��·
       
        distanceFactor = 0.2;
        break;
    case 3:  // ��·
        
        distanceFactor = 0.3;
        break;
    case 4:  // ����
        
        distanceFactor = 0.1;
        break;
    case 5:  // ���˻�
        
        distanceFactor = 0.4;
        break;

    default:
        return -1;  // ��Ч�����䷽ʽ
    }

    // ��������������;���ķ���
    double weightCost = pkg->weight * weightFactor;
    double volumeCost = pkg->volume * volumeFactor;
    double distanceCost = pkg->distance * distanceFactor;

    // �����ܷ���
    double totalCost = baseCost + weightCost + volumeCost + distanceCost;
    double discountFactor[5] = { 0.9,0.8,0.7,0.6,0.5 };
    totalCost = totalCost * discountFactor[user_level];
    return totalCost;
}
// ������Ԥ������ʵ��
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
            printf("���棺���� %c ��ʹ�� %.2f%%���ӽ������ɣ�\n", 'A' + i, usage * 100);
        }
    }
}