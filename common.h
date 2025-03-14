#ifndef _COMMON_H_
#define _COMMON_H_


#define MAX_NAME_LEN 10
#define MAX_INFO_LEN 100
#define MAX_PASSWD_LEN 20

int user_num;
int package_num;


typedef struct package
{
    int id;
    int weight;
    int transport;
    int cost;
    int status;//包裹状态 0：损坏  1：正常 2：丢失

    char sender_name[MAX_NAME_LEN];
    char receiver_name[MAX_NAME_LEN];
    char info[MAX_INFO_LEN];

    
    struct package* package_fd;//next
    struct package* package_bk;
}package;


typedef struct user
{
    int userid;//身份码
    int privilege;//0:管理员 1:快递员 2：用户
    int level;

    char username[MAX_NAME_LEN];
    char password[MAX_PASSWD_LEN];
    char phone[11];
    char info[MAX_INFO_LEN];

    struct user* user_fd;//next
    struct user* user_bk;

    package packages[MAX_INFO_LEN];//一个用户所拥有的包裹
}user;


package* package_head;
user* user_head;
user* cur_user;

user* login();//登录
int regist();//注册


int package_ctrl();//包裹管理
int user_ctrl();//用户管理
void queryUser(user*);//查询用户
void deleteUser(user**);//删除用户

void inform_user(user*);//通知用户
void default_menu();
void admin_menu();
void user_menu();
void printUserInfo(user*);//打印用户信息

void constructor();//程序启动时初始化链表
void destructor();//程序结束时保存链表到文件
void save_packages();
void save_users();

int keycheck(char* str);
int keygen(char* str);

int getValidInt(int min, int max);
void getValidString(char* str, int max_len);

user* admin_revise(user* head);
void user_revise(user* userToModify, user* userHead);
#endif