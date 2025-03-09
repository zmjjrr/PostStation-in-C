
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include "common.h"

static int user_id = 1;
static int admin_id = 1;
static int postman_id = 1;

#define next user_fd


// 密码验证
static user* password(user* newman) {
    while (1) {
        printf("please input your password(must include num,words(include lower and supper),less than 20): ");
        fgets(newman->password, MAX_PASSWD_LEN, stdin);
        // 去除换行符
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
            printf("your password is so low,please try again\n");
            memset(newman->password, 0, sizeof(newman->password));
        }
    }
    return newman;
}


//登录
user* _login(user* head) {
    char name[20], password[20];
    user* p;
    int i = 0;
    while (1) {
        printf("请输入君の名: ");
        scanf("%s", name);
        while (getchar() != '\n');
        p = head;
        int found = 0;
        while (p) {
            if (strcmp(p->username, name) == 0) {
                found = 1;
                while (i < 3) {
                    printf("请输入密码: ");
                    scanf("%s", password);
                    while (getchar() != '\n');
                    if (strcmp(p->password, password) == 0) {
                        printf("欢迎登录\n");//待加入authentication逻辑
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

extern user* user_head;
user* login()
{
    return _login(user_head);
    //return a randomized authentication token?
}
