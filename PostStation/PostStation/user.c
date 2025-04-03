#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include"common.h"
static int user_id = 1;
static int admin_id = 1;
static int postman_id = 1;

#define next user_fd
#define phone_str phone
static void reassignIds(user *head) {
    int newId = 1;
    user *current = head;
    while (current != NULL) {
        current->userid = newId;
        newId++;
        current = current->next;
    }
}

int is_username_duplicate(user* head, const char* username, user* current) {
    user* pi = head;
    while (pi) {
        if (pi != current && strcmp(pi->username, username) == 0) {
            return 1;
        }
        pi = pi->user_fd;
    }
    return 0;
}
user* creat(void) {
    user* head;
    head = (user*)calloc(1,sizeof(user));
    head->next = NULL;
    return head;
}

user* add(user* head, user* newman) {
    if (head == NULL) {
        return newman;
    }
    user* p = head;
    while (p->user_fd != NULL) {
        p = p->user_fd;
    }
    p->user_fd = newman;
    newman->user_fd = NULL;
    return head;
}

// ������֤
static user* password(user* newman) {
    while (1) {
        printf("�������������(����������ֺʹ�Сд)\n");
        getValidString(newman->password, MAX_PASSWD_LEN);
        int has_digit = 0, has_upper = 0, has_lower = 0, has_other = 0;
        for (int i = 0; newman->password[i] != '\0'; i++) {
            if (isdigit(newman->password[i])) {
                has_digit = 1;
            } else if (isupper(newman->password[i])) {
                has_upper = 1;
            } else if (islower(newman->password[i])) {
                has_lower = 1;
            } else {
                has_other = 1;
            }
        }
        if (has_digit && has_upper && has_lower && !has_other) {
            break;
        }
        printf("����̫ѷ�ˣ��ٻ����µ�\n");
    }
    return newman;
}

// �����û���Ϣ
user* perfect(user* newman) {
    printf("�������������o(*�����*)o�������� %d): ", MAX_NAME_LEN);
    char namebuffer[MAX_NAME_LEN];
    getValidString(namebuffer, MAX_NAME_LEN);
    if(is_username_duplicate(user_head, namebuffer, newman)){
        printf("�û����Ѵ��ڣ�����������\n");
        return NULL;
    }
    strcpy(newman->username, namebuffer);
    password(newman);
    while (1) {
        int m = 0;
        char input[20]; // ����һ���㹻����������洢����
        printf("��������ĵ绰: ");
        if (scanf("%19s", input) != 1) {
            while (getchar() != '\n');
            printf("��ƭ�ˣ��绰��������.[��?�৥?? ��]\n");
            continue;
        }
        while (getchar() != '\n'); // ������뻺����

        if (strlen(input) != 11) {
            printf("��ƭ�ˣ��绰��������.[��?�৥?? ��]\n");
            continue;
        }

        for (int i = 0; i < 11; i++) {
            if (isdigit(input[i])) {
                m++;
            }
        }
        if (m == 11) {
            strcpy(newman->phone, input); // ����֤ͨ�������븴�Ƶ� newman->phone
            break;
        }
        else {
            printf("��ƭ�ˣ��绰��������.[��?�৥?? ��]\n");
        }
    }
    return newman;
}


// �������û�����ӵ�������
user* build(user* head) {
    user* newman = (user*)calloc(1,sizeof(user));
    int p;
    while (1) {
        printf("������������(0:����Ա,1:�û�,2:���Ա): ");
        p = getValidInt(0, 2);
        if (p == 0) {
            printf("������������\n");
            char s[40];
            getValidString(s, 35);
            if (keycheck(s)) {
                newman->privilege = 0;
                newman->userid = ++user_num;
                break;
            }
            else {
                printf("���������\n");
            }
        }
        else if (p == 1) {
            newman->privilege = 1;
            newman->userid = ++user_num;
            break;
        }
        else if (p == 2) {
            newman->privilege = 2;
            newman->userid = ++user_num;
            break;
        }
        else {
            printf("����������ٴγ���\n");
        }
    }
    newman = perfect(newman);
    if (newman == NULL) {
        return head;
    }

    newman->level = 0;
    newman->total_cost = 0;
    newman->user_fd = NULL;
    head = add(head, newman);
    record_history(newman->username, "ע��");
    return head;
}

user* user_head;
int regist() {
    user_head = build(user_head);
    return 0;
}

user* admin_revise(user* head) {
    char nameToModify[MAX_NAME_LEN];
    printf("������Ҫ�޸���Ϣ���û���ID: ");
    int id = getValidInt(1, user_num);

    user* current = head;
    while (current != NULL) {
        if (current->userid == user_id) {
            user_revise(current, head);
            return head;
        }
        current = current->user_fd;
    }
    printf("δ�ҵ�ָ���û������û���\n");
    return head;
}
void user_revise(user* userToModify, user* userHead) {
    if (userToModify == NULL) {
        printf("������û�ָ��Ϊ�գ��޷��޸���Ϣ��\n");
        return;
    }
    int choice;
    while (1) {
        printf("�ɽ��������޸ģ�\n");
        printf("1. �޸�����\n");
        printf("2. �޸�����\n");
        printf("3. �޸ĵ绰����\n");
        printf("4. �޸��û����\n");
        printf("5. �˳��޸�\n");
        choice = getValidInt(1, 5);

        switch (choice) {
        case 1: {
            char new_name[MAX_NAME_LEN];
            while (1) {
                printf("�������µ�����(��� %d ���ַ�): ", MAX_NAME_LEN);
                getValidString(new_name, MAX_NAME_LEN);
                char old_name[20];
                strcpy(old_name, userToModify->username);
                if (!is_username_duplicate(userHead, new_name, userToModify)) {
                    strcpy(userToModify->username, new_name);
                    printf("�����޸ĳɹ���\n");
                    record_history(old_name, "���޸�");
                    record_history(new_name, "����");
                    break;
                }
                printf("�û����ظ�,���޸ģ�\n");
            }
            break;
        }
        case 2:
            password(userToModify);
            printf("�����޸ĳɹ���\n");
            record_history(userToModify, "�޸�����");
            break;
        case 3: {
            char new_phone[12];
            while (1) {
                printf("�������µĵ绰����: ");
                getValidString(new_phone, sizeof(new_phone)-1);
                int valid = 1;
                if (strlen(new_phone) != 11) {
                    valid = 0;
                } else {
                    for (int i = 0; i < 11; i++) {
                        if (!isdigit(new_phone[i])) {
                            valid = 0;
                            break;
                        }
                    }
                }
                if (valid) {
                    strcpy(userToModify->phone, new_phone);
                    record_history(userToModify, "�޸ĵ绰");
                    printf("�绰�����޸ĳɹ���\n");
                    break;
                } else {
                    printf("��Ч���룬������ 11 λ���ֵĵ绰���롣\n");
                }
            }
            break;
        }
        case 4:
        {
            char new_info[MAX_NAME_LEN];
            printf("�������µ��û����(��� %d ���ַ�): ", MAX_INFO_LEN);
            getValidString(new_info, MAX_INFO_LEN);
            strcpy(userToModify->info, new_info);
            record_history(userToModify, "�޸ļ��");
            printf("�û�����޸ĳɹ���\n");
            break;
        }
        case 5:
            printf("�˳��޸Ĳ�����\n");
            return;
        default:
            printf("��Чѡ�����������롣\n");
        }
    }
}

user* _login(user* head) {
    char name[20], password[20];
    user* p;
    int i = 0;
    while (1) {
        printf("�����������: ");
        getValidString(name, MAX_NAME_LEN);
        p = head;
        int found = 0;
        while (p) {
            if (strcmp(p->username, name) == 0) {
                found = 1;
                while (i < 3) {
                    printf("����������: ");
                    getValidString(password, MAX_PASSWD_LEN);
                    if (strcmp(p->password, password) == 0) {
                        printf("��ӭ��¼\n");
                        return p;
                    }
                    else {
                        i++;
                        printf("����������������룬�㻹�� %d �γ��Ի���\n", 3 - i);
                    }
                }
                printf("�������������࣬���ٴε�¼\n");
                return NULL;
            }
            p = p->next;
        }

        if (!found) {
            printf("δ�ҵ����û�\n");
            return NULL;
        }
    }
}


user* login()
{
    return _login(user_head);
}

void deleteUser(user **head) {
    if (*head == NULL) {
        printf("û���û���Ϣ���޷�ɾ����\n");
        return;
    }
    int id;
    printf("������Ҫɾ�����û� ID: ");
    id = getValidInt(1, user_num);

    user *current = *head;
    user *prev = NULL;

    while (current != NULL && current->userid != id) {
        prev = current;
        current = current->user_fd;
    }

    if (current == NULL) {
        printf("δ�ҵ����û� ID��ɾ��ʧ�ܣ�\n");
        return;
    }

    if (current->privilege == 0)
    {
        printf("����Ա�˺��޷�ɾ��!");
        return;
    }

    if (prev == NULL) {
        *head = current->user_fd;
    } else {
        prev->user_fd = current->user_fd;
    }
    record_history(current->username, "delet");
    free(current);
    user_num--;
    reassignIds(*head);
    printf("�û�ɾ���ɹ���\n");
}

// ��ӡ�����û���Ϣ�ĺ���
void printUserInfo(user *current) {
    printf("�û� ID: %d\n", current->userid);
    printf("Ȩ��: %d\n", current->privilege);
    printf("�ȼ�: %d\n", current->level);
    printf("�û���: %s\n", current->username);
    printf("����: %s\n", current->password);
    printf("�ֻ�����: %s\n", current->phone);
    printf("�û���Ϣ: %s\n", current->info);
    printf("------------------------\n");
}
// ��ѯ�û�����
void queryUser(user *head) {
    if (head == NULL) {
        printf("û���û���Ϣ��\n");
        return;
    }

    int choice;
    do {
        printf("\n=== �û���ѯ�˵� ===\n");
        printf("1. ��ʾȫ���û�\n");
        printf("2. ���� ID ��ѯ\n");
        printf("3. �����û�����ѯ\n");
        printf("4. ����Ȩ�޲�ѯ\n");
        printf("5. �������˵�\n");
        printf("���������ѡ��: ");
        choice = getValidInt(1, 5);

        switch (choice) {
            case 1: {
                user *current = head;
                while (current != NULL) {
                    printUserInfo(current);
                    current = current->user_fd;
                }
                break;
            }
            case 2: {
                int id;
                printf("������Ҫ��ѯ���û� ID: ");
                id = getValidInt(1, user_num);

                user *current = head;
                int found = 0;
                while (current != NULL) {
                    if (current->userid == id) {
                        printUserInfo(current);
                        found = 1;
                    }
                    current = current->user_fd;
                }
                if (!found) {
                    printf("δ�ҵ����û� ID ���û���\n");
                }
                break;
            }
            case 3: {
                char username[MAX_NAME_LEN];
                printf("������Ҫ��ѯ���û���: ");
                getValidString(username, MAX_NAME_LEN);

                user *current = head;
                int found = 0;
                while (current != NULL) {
                    if (strcmp(current->username, username) == 0) {
                        printUserInfo(current);
                        found = 1;
                    }
                    current = current->user_fd;
                }
                if (!found) {
                    printf("δ�ҵ����û������û���\n");
                }
                break;
            }
            case 4: {
                int privilege;
                printf("������Ҫ��ѯ���û�Ȩ�� (0: ����Ա, 1: �û�, 2: ���Ա): ");
                privilege = getValidInt(0, 2);

                user *current = head;
                int found = 0;
                while (current != NULL) {
                    if (current->privilege == privilege) {
                        printUserInfo(current);
                        found = 1;
                    }
                    current = current->user_fd;
                }
                if (!found) {
                    printf("δ�ҵ���Ȩ�޵��û���\n");
                }
                break;
            }
            case 5:
                printf("�������˵���\n");
                break;
            default:
                printf("��Ч��ѡ�����������룡\n");
        }
    } while (choice != 5);
}
void update_user_level(user* u) {
    if (u->total_cost >= 500 && u->level < 4) {
        u->level = 4;
        printf("�û� %s ���������ȼ� 4��\n", u->username);
    }
    else if (u->total_cost >= 300 && u->level < 3) {
        u->level = 3;
        printf("�û� %s ���������ȼ� 3��\n", u->username);
    }
    else if (u->total_cost >= 150 && u->level < 2) {
        u->level = 2;
        printf("�û� %s ���������ȼ� 2��\n", u->username);
    }
    else if (u->total_cost >= 50&& u->level < 1) {
        u->level = 1;
        printf("�û� %s ���������ȼ� 1��\n", u->username);
    }
}