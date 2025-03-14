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
        scanf("%20s", newman->password);
        while(getchar()!= '\n');
        size_t len = strlen(newman->password);
        if (len > 0 && newman->password[len - 1] == '\n') {
            newman->password[len - 1] = '\0';
        }
        int i = 0, a = 0, b = 0, c = 0, d = 0;
        for (i = 0; newman->password[i] != '\0'; i++) {
            if (isdigit(newman->password[i])) {
                a++;
            }
            else if (isupper(newman->password[i])) {
                b++;
            }
            else if (islower(newman->password[i])) {
                c++;
            }
            else {
                d++;
            }
        }
        if (a != 0 && b != 0 && c != 0 && d == 0) {
            break;
        }
        else {
            printf("����̫ѷ�ˣ��ٻ����µ�\n");
            memset(newman->password, 0, sizeof(newman->password));
        }
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
    scanf("%20s", nameToModify);
    while (getchar() != '\n');
    user* current = head;
    while (current != NULL) {
        if (strcmp(current->username, nameToModify) == 0) {
            printf("�ҵ��û����ɽ��������޸ģ�\n");
            printf("1. �޸�����\n");
            printf("2. �޸�����\n");
            int choice;
            while (1) {
                printf("��ѡ��Ҫ�޸ĵ���(1��2): ");
                if (scanf("%d", &choice) != 1) {
                    while (getchar() != '\n');
                    printf("��Ч���룬������1��2��\n");
                    continue;
                }
                while (getchar() != '\n');
                if (choice == 1) {
                    printf("�������µ�����(��� %d ���ַ�): ", MAX_NAME_LEN);
                    scanf("%9s", current->username);
                    while (getchar() != '\n');
                    user* pi = head;
                    int m=0;
                while (pi) {
                    
                    if (strcmp(pi->username, current->username) == 0) {
                        printf("�û����ظ�,���޸ģ�\n");
                        m=1;
                    }
                    else {
                        pi = pi->user_fd;
                    }
                }
                if (m==1){
                    break;
                }
                    printf("�����޸ĳɹ���\n");
                    break;
                }
                else if (choice == 2) {
                    password(current);
                    printf("�����޸ĳɹ���\n");
                    break;
                }
                else {
                    printf("��Чѡ�����������롣\n");
                }
            }
            return head;
        }
        current = current->user_fd;
    }
    printf("δ�ҵ�ָ���û������û���\n");
    return head;
}
void user_revise(user* userToModify) {
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
        printf("��ѡ��Ҫ�޸ĵ���(1 - 4): ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("��Ч���룬������ 1 - 4 �����֡�\n");
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
        case 1:
            printf("�������µ�����(��� %d ���ַ�): ", MAX_NAME_LEN);
            if (scanf("%9s", userToModify->username) != 1) {
                while (getchar() != '\n');
                printf("������Ч������δ�޸ġ�\n");
            }
            else {
                while (getchar() != '\n');
                printf("�����޸ĳɹ���\n");
            }
            break;
        case 2:
            password(userToModify);
            printf("�����޸ĳɹ���\n");
            break;
        case 3:
            while (1) {
                int m = 0;
                printf("�������µĵ绰����: ");
                if (scanf("%11s", userToModify->phone_str) != 1) {
                    while (getchar() != '\n');
                    printf("��Ч���룬������ 11 λ���ֵĵ绰���롣\n");
                }
                else {
                    for (int i = 0; i < 11; i++) {
                        if (isdigit(userToModify->phone_str[i])) {
                            m++;
                        }
                    }
                    if (m == 11) {
                        printf("�绰�����޸ĳɹ���\n");
                        break;
                    }
                    else {
                        printf("��Ч���룬������ 11 λ���ֵĵ绰���롣\n");
                    }
                }
            }
            break;
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