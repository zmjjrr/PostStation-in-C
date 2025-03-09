
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

user* add(user* head, user* newman) {//insert at the end
    user* p = head;
    if (head == NULL) {
        head = newman;
        head->next = NULL;
        return head;
    }
    else {
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = newman;
        newman->next = NULL;
        return head;
    }
}

static user* password(user* newman) {
    while (1) {
        printf("please input your password(must include num,words(include lower and supper),less than 20): ");
        fgets(newman->password, MAX_PASSWD_LEN, stdin);
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

user* perfect(user* newman) {
    printf("please input your name(least than %d): ", MAX_NAME_LEN);
    if (scanf("%9s", newman->username) != 1) {
        while (getchar() != '\n');
        printf("Invalid input for name.\n");
        return NULL;
    }
    while (getchar() != '\n');
    password(newman);
    while (1) {
        int m = 0;
        printf("please input your phonenumber: ");
        if (scanf("%11s", newman->phone_str) != 1) {
            while (getchar() != '\n');
            printf("Invalid input for phone number.\n");
        }
        for (int i = 0;i < 11;i++) {
            if (isdigit(newman->phone_str[i])) {
                m++;
           }
        }
        if (m == 11) {
            break;
        }
        else {
            printf("Invalid input for phone number.\n");
        }
    }
    return newman;
}

user* build(user* head) {
    user* newman = (user*)malloc(sizeof(user));
    int p;
    while (1) {
        printf("please input your role(0:admin,1:user,2:postman): ");
        if (scanf("%d", &p) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        while (getchar() != '\n');
        if (p == 0) {
            newman->privilege = 0;
            newman->userid = admin_id++;
            break;
        }
        else if (p == 1) {
            newman->privilege = 1;
            newman->userid = user_id++;
            break;
        }
        else if (p == 2) {
            newman->privilege = 2;
            newman->userid = postman_id++;
            break;
        }
        else {
            printf("sorry, invalid role. Please try again.\n");
        }
    }
    newman = perfect(newman);
    if (newman == NULL) {
        free(newman);
        return head;
    }
    newman->level = 0;
    newman->next = NULL;
    head = add(head, newman);
    return head;
}


extern user* user_head;
int regist(){
    user_head = build(user_head);
}

