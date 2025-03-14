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

// 密码验证
static user* password(user* newman) {
    while (1) {
        printf("告诉我你的密码(必须包含数字和大小写)\n");
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
        printf("密码太逊了，再换个新的\n");
    }
    return newman;
}

// 完善用户信息
user* perfect(user* newman) {
    printf("请输入君の名字o(*￣︶￣*)o（不超过 %d): ", MAX_NAME_LEN);
    if (scanf("%9s", newman->username) != 1) {
        while (getchar() != '\n');
        printf("Invalid input for name.\n");
        return NULL;
    }
    while (getchar() != '\n');
    password(newman);
    while (1) {
        int m = 0;
        char input[20]; // 定义一个足够大的数组来存储输入
        printf("告诉我你的电话: ");
        if (scanf("%19s", input) != 1) {
            while (getchar() != '\n');
            printf("你骗人，电话不是这样.[○?｀Д?? ○]\n");
            continue;
        }
        while (getchar() != '\n'); // 清除输入缓冲区

        if (strlen(input) != 11) {
            printf("你骗人，电话不是这样.[○?｀Д?? ○]\n");
            continue;
        }

        for (int i = 0; i < 11; i++) {
            if (isdigit(input[i])) {
                m++;
            }
        }
        if (m == 11) {
            strcpy(newman->phone, input); // 将验证通过的输入复制到 newman->phone
            break;
        }
        else {
            printf("你骗人，电话不是这样.[○?｀Д?? ○]\n");
        }
    }
    return newman;
}

// 分配用户ID
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

// 创建新用户并添加到链表中
user* build(user* head) {
    user* newman = (user*)malloc(sizeof(user));
    int p;
    while (1) {
        printf("请输入你的身份(0:管理员,1:用户,2:快递员): ");
        if (scanf("%d", &p) != 1) {
            while (getchar() != '\n');
            printf("错误！请输入以上数字\n");
            continue;
        }
        while (getchar() != '\n');
        if (p == 0) {
            printf("请输入邀请码\n");
            char s[33];
            scanf("%32s", s);
            while (getchar() != '\n');
            if (keycheck(s)) {
                newman->privilege = 0;
                newman->userid = admin_id++;
                break;
            }
            else {
                printf("邀请码错误\n");
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
            printf("输入错误，请再次尝试\n");
        }
    }
    newman = perfect(newman);
    if (newman == NULL) {
        free(newman);
        return head;
    }
    // 检查用户名是否重复
    user* pi = head->user_fd;
    while (pi) {
        if (strcmp(pi->username, newman->username) == 0) {
            printf("用户名重复,请修改：\n");
            if (scanf("%9s", newman->username) != 1) {
                while (getchar() != '\n');
                printf("Invalid input for name.\n");
                free(newman);
                return head;
            }
            while (getchar() != '\n');
            pi = head->user_fd; // 重新从头开始检查
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
    printf("请输入要修改信息的用户名: ");
    getValidString(nameToModify, MAX_NAME_LEN);

    user* current = head;
    while (current != NULL) {
        if (strcmp(current->username, nameToModify) == 0) {
            printf("找到用户，可进行如下修改：\n");
            printf("1. 修改名字\n");
            printf("2. 修改密码\n");
            int choice = getValidInt(1, 2);

            switch (choice) {
            case 1: {
                char new_name[MAX_NAME_LEN];
                while (1) {
                    printf("请输入新的名字(最多 %d 个字符): ", MAX_NAME_LEN);
                    getValidString(new_name, MAX_NAME_LEN);
                    if (!is_username_duplicate(head, new_name, current)) {
                        strcpy(current->username, new_name);
                        printf("名字修改成功。\n");
                        break;
                    }
                    printf("用户名重复,请修改：\n");
                }
                break;
            }
            case 2:
                password(current);
                printf("密码修改成功。\n");
                break;
            }
            return head;
        }
        current = current->user_fd;
    }
    printf("未找到指定用户名的用户。\n");
    return head;
}
void user_revise(user* userToModify, user* userHead) {
    if (userToModify == NULL) {
        printf("传入的用户指针为空，无法修改信息。\n");
        return;
    }
    int choice;
    while (1) {
        printf("可进行如下修改：\n");
        printf("1. 修改名字\n");
        printf("2. 修改密码\n");
        printf("3. 修改电话号码\n");
        printf("4. 退出修改\n");
        choice = getValidInt(1, 4);

        switch (choice) {
        case 1: {
            char new_name[MAX_NAME_LEN];
            while (1) {
                printf("请输入新的名字(最多 %d 个字符): ", MAX_NAME_LEN);
                getValidString(new_name, MAX_NAME_LEN);
                if (!is_username_duplicate(userHead, new_name, userToModify)) {
                    strcpy(userToModify->username, new_name);
                    printf("名字修改成功。\n");
                    break;
                }
                printf("用户名重复,请修改：\n");
            }
            break;
        }
        case 2:
            password(userToModify);
            printf("密码修改成功。\n");
            break;
        case 3: {
            char new_phone[12];
            while (1) {
                printf("请输入新的电话号码: ");
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
                    printf("电话号码修改成功。\n");
                    break;
                } else {
                    printf("无效输入，请输入 11 位数字的电话号码。\n");
                }
            }
            break;
        }
        case 4:
            printf("退出修改操作。\n");
            return;
        default:
            printf("无效选择，请重新输入。\n");
        }
    }
}

user* _login(user* head) {
    char name[20], password[20];
    user* p;
    int i = 0;
    while (1) {
        printf("请输入君の名: ");
        scanf("%20s", name);
        while (getchar() != '\n');
        p = head;
        int found = 0;
        while (p) {
            if (strcmp(p->username, name) == 0) {
                found = 1;
                while (i < 3) {
                    printf("请输入密码: ");
                    scanf("%20s", password);//????
                    while (getchar() != '\n');
                    if (strcmp(p->password, password) == 0) {
                        printf("欢迎登录\n");
                        return p;
                    }
                    else {
                        i++;
                        printf("密码错误，请重新输入，你还有 %d 次尝试机会\n", 3 - i);
                    }
                }
                printf("密码错误次数过多，请再次登录\n");
                return NULL;
            }
            p = p->next;
        }

        if (!found) {
            printf("未找到该用户，请重新输入\n");
        }
    }
}


user* login()
{
    return _login(user_head);
}