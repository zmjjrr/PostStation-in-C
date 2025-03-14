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
    head = (user*)malloc(sizeof(user));
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
    if (scanf("%9s", newman->username) != 1) {
        while (getchar() != '\n');
        printf("Invalid input for name.\n");
        return NULL;
    }
    while (getchar() != '\n');
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

// �����û�ID
int id(user* head, int m) {
    user* p = head->user_fd;
    int max_id = 0;
    while (p) {
        if (p->privilege == m) {
            if (p->userid > max_id) {
                max_id = p->userid;
            }
        }
        p = p->user_fd;
    }
    return max_id + 1;
}

// �������û�����ӵ�������
user* build(user* head) {
    user* newman = (user*)malloc(sizeof(user));
    int p;
    while (1) {
        printf("������������(0:����Ա,1:�û�,2:���Ա): ");
        if (scanf("%d", &p) != 1) {
            while (getchar() != '\n');
            printf("������������������\n");
            continue;
        }
        while (getchar() != '\n');
        if (p == 0) {
            printf("������������\n");
            char s[33];
            scanf("%32s", s);
            while (getchar() != '\n');
            if (keycheck(s)) {
                newman->privilege = 0;
                newman->userid = admin_id++;
                break;
            }
            else {
                printf("���������\n");
            }
        }
        else if (p == 1) {
            newman->privilege = 1;
            newman->userid = id(head, 1);
            break;
        }
        else if (p == 2) {
            newman->privilege = 2;
            newman->userid = id(head, 2);
            break;
        }
        else {
            printf("����������ٴγ���\n");
        }
    }
    newman = perfect(newman);
    if (newman == NULL) {
        free(newman);
        return head;
    }
    // ����û����Ƿ��ظ�
    user* pi = head->user_fd;
    while (pi) {
        if (strcmp(pi->username, newman->username) == 0) {
            printf("�û����ظ�,���޸ģ�\n");
            if (scanf("%9s", newman->username) != 1) {
                while (getchar() != '\n');
                printf("Invalid input for name.\n");
                free(newman);
                return head;
            }
            while (getchar() != '\n');
            pi = head->user_fd; // ���´�ͷ��ʼ���
        }
        else {
            pi = pi->user_fd;
        }
    }
    newman->level = 0;
    newman->user_fd = NULL;
    head = add(head, newman);
    return head;
}

user* user_head;
int regist() {
    user_head = build(user_head);
    return 0;
}

user* admin_revise(user* head) {
    char nameToModify[MAX_NAME_LEN];
    printf("������Ҫ�޸���Ϣ���û���: ");
    getValidString(nameToModify, MAX_NAME_LEN);

    user* current = head;
    while (current != NULL) {
        if (strcmp(current->username, nameToModify) == 0) {
            printf("�ҵ��û����ɽ��������޸ģ�\n");
            printf("1. �޸�����\n");
            printf("2. �޸�����\n");
            int choice = getValidInt(1, 2);

            switch (choice) {
            case 1: {
                char new_name[MAX_NAME_LEN];
                while (1) {
                    printf("�������µ�����(��� %d ���ַ�): ", MAX_NAME_LEN);
                    getValidString(new_name, MAX_NAME_LEN);
                    if (!is_username_duplicate(head, new_name, current)) {
                        strcpy(current->username, new_name);
                        printf("�����޸ĳɹ���\n");
                        break;
                    }
                    printf("�û����ظ�,���޸ģ�\n");
                }
                break;
            }
            case 2:
                password(current);
                printf("�����޸ĳɹ���\n");
                break;
            }
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
        printf("4. �˳��޸�\n");
        choice = getValidInt(1, 4);

        switch (choice) {
        case 1: {
            char new_name[MAX_NAME_LEN];
            while (1) {
                printf("�������µ�����(��� %d ���ַ�): ", MAX_NAME_LEN);
                getValidString(new_name, MAX_NAME_LEN);
                if (!is_username_duplicate(userHead, new_name, userToModify)) {
                    strcpy(userToModify->username, new_name);
                    printf("�����޸ĳɹ���\n");
                    break;
                }
                printf("�û����ظ�,���޸ģ�\n");
            }
            break;
        }
        case 2:
            password(userToModify);
            printf("�����޸ĳɹ���\n");
            break;
        case 3: {
            char new_phone[12];
            while (1) {
                printf("�������µĵ绰����: ");
                getValidString(new_phone, sizeof(new_phone));
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
                    printf("�绰�����޸ĳɹ���\n");
                    break;
                } else {
                    printf("��Ч���룬������ 11 λ���ֵĵ绰���롣\n");
                }
            }
            break;
        }
        case 4:
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
        scanf("%20s", name);
        while (getchar() != '\n');
        p = head;
        int found = 0;
        while (p) {
            if (strcmp(p->username, name) == 0) {
                found = 1;
                while (i < 3) {
                    printf("����������: ");
                    scanf("%20s", password);//????
                    while (getchar() != '\n');
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
            printf("δ�ҵ����û�������������\n");
        }
    }
}


user* login()
{
    return _login(user_head);
}