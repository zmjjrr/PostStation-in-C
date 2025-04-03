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
    char namebuffer[MAX_NAME_LEN];
    getValidString(namebuffer, MAX_NAME_LEN);
    if(is_username_duplicate(user_head, namebuffer, newman)){
        printf("用户名已存在，请重新输入\n");
        return NULL;
    }
    strcpy(newman->username, namebuffer);
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


// 创建新用户并添加到链表中
user* build(user* head) {
    user* newman = (user*)calloc(1,sizeof(user));
    int p;
    while (1) {
        printf("请输入你的身份(0:管理员,1:用户,2:快递员): ");
        p = getValidInt(0, 2);
        if (p == 0) {
            printf("请输入邀请码\n");
            char s[40];
            getValidString(s, 35);
            if (keycheck(s)) {
                newman->privilege = 0;
                newman->userid = ++user_num;
                break;
            }
            else {
                printf("邀请码错误\n");
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
            printf("输入错误，请再次尝试\n");
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
    record_history(newman->username, "注册");
    return head;
}

user* user_head;
int regist() {
    user_head = build(user_head);
    return 0;
}

user* admin_revise(user* head) {
    char nameToModify[MAX_NAME_LEN];
    printf("请输入要修改信息的用户的ID: ");
    int id = getValidInt(1, user_num);

    user* current = head;
    while (current != NULL) {
        if (current->userid == user_id) {
            user_revise(current, head);
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
        printf("4. 修改用户简介\n");
        printf("5. 退出修改\n");
        choice = getValidInt(1, 5);

        switch (choice) {
        case 1: {
            char new_name[MAX_NAME_LEN];
            while (1) {
                printf("请输入新的名字(最多 %d 个字符): ", MAX_NAME_LEN);
                getValidString(new_name, MAX_NAME_LEN);
                char old_name[20];
                strcpy(old_name, userToModify->username);
                if (!is_username_duplicate(userHead, new_name, userToModify)) {
                    strcpy(userToModify->username, new_name);
                    printf("名字修改成功。\n");
                    record_history(old_name, "被修改");
                    record_history(new_name, "生成");
                    break;
                }
                printf("用户名重复,请修改：\n");
            }
            break;
        }
        case 2:
            password(userToModify);
            printf("密码修改成功。\n");
            record_history(userToModify, "修改密码");
            break;
        case 3: {
            char new_phone[12];
            while (1) {
                printf("请输入新的电话号码: ");
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
                    record_history(userToModify, "修改电话");
                    printf("电话号码修改成功。\n");
                    break;
                } else {
                    printf("无效输入，请输入 11 位数字的电话号码。\n");
                }
            }
            break;
        }
        case 4:
        {
            char new_info[MAX_NAME_LEN];
            printf("请输入新的用户简介(最多 %d 个字符): ", MAX_INFO_LEN);
            getValidString(new_info, MAX_INFO_LEN);
            strcpy(userToModify->info, new_info);
            record_history(userToModify, "修改简介");
            printf("用户简介修改成功。\n");
            break;
        }
        case 5:
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
        getValidString(name, MAX_NAME_LEN);
        p = head;
        int found = 0;
        while (p) {
            if (strcmp(p->username, name) == 0) {
                found = 1;
                while (i < 3) {
                    printf("请输入密码: ");
                    getValidString(password, MAX_PASSWD_LEN);
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
            printf("未找到该用户\n");
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
        printf("没有用户信息，无法删除！\n");
        return;
    }
    int id;
    printf("请输入要删除的用户 ID: ");
    id = getValidInt(1, user_num);

    user *current = *head;
    user *prev = NULL;

    while (current != NULL && current->userid != id) {
        prev = current;
        current = current->user_fd;
    }

    if (current == NULL) {
        printf("未找到该用户 ID，删除失败！\n");
        return;
    }

    if (current->privilege == 0)
    {
        printf("管理员账号无法删除!");
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
    printf("用户删除成功！\n");
}

// 打印单个用户信息的函数
void printUserInfo(user *current) {
    printf("用户 ID: %d\n", current->userid);
    printf("权限: %d\n", current->privilege);
    printf("等级: %d\n", current->level);
    printf("用户名: %s\n", current->username);
    printf("密码: %s\n", current->password);
    printf("手机号码: %s\n", current->phone);
    printf("用户信息: %s\n", current->info);
    printf("------------------------\n");
}
// 查询用户函数
void queryUser(user *head) {
    if (head == NULL) {
        printf("没有用户信息！\n");
        return;
    }

    int choice;
    do {
        printf("\n=== 用户查询菜单 ===\n");
        printf("1. 显示全部用户\n");
        printf("2. 根据 ID 查询\n");
        printf("3. 根据用户名查询\n");
        printf("4. 根据权限查询\n");
        printf("5. 返回主菜单\n");
        printf("请输入你的选择: ");
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
                printf("请输入要查询的用户 ID: ");
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
                    printf("未找到该用户 ID 的用户！\n");
                }
                break;
            }
            case 3: {
                char username[MAX_NAME_LEN];
                printf("请输入要查询的用户名: ");
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
                    printf("未找到该用户名的用户！\n");
                }
                break;
            }
            case 4: {
                int privilege;
                printf("请输入要查询的用户权限 (0: 管理员, 1: 用户, 2: 快递员): ");
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
                    printf("未找到该权限的用户！\n");
                }
                break;
            }
            case 5:
                printf("返回主菜单！\n");
                break;
            default:
                printf("无效的选择，请重新输入！\n");
        }
    } while (choice != 5);
}
void update_user_level(user* u) {
    if (u->total_cost >= 500 && u->level < 4) {
        u->level = 4;
        printf("用户 %s 已升级到等级 4！\n", u->username);
    }
    else if (u->total_cost >= 300 && u->level < 3) {
        u->level = 3;
        printf("用户 %s 已升级到等级 3！\n", u->username);
    }
    else if (u->total_cost >= 150 && u->level < 2) {
        u->level = 2;
        printf("用户 %s 已升级到等级 2！\n", u->username);
    }
    else if (u->total_cost >= 50&& u->level < 1) {
        u->level = 1;
        printf("用户 %s 已升级到等级 1！\n", u->username);
    }
}