#ifndef _COMMON_H_
#define _COMMON_H_


#define MAX_NAME_LEN 50
#define MAX_INFO_LEN 100
#define MAX_PASSWD_LEN 20
#define MAX_PACAKGE_NUM 1000
#define SHELF_WARNING_THRESHOLD 0.8  // 货架预警阈值，达到 80% 时预警
#define MAX_COMMAD_LEN 100
#define MAX_TOKEN_LEN 20
int user_num;
int package_num;

typedef struct package
{
    int id;
    double weight;
    int transport;
    double cost;
    int type;//0：普通快递 1：生鲜快递 2：贵重快递 3：大件快递
    int status;//包裹状态 0：损坏  1：正常 2：丢失
    int T_status;//0：待寄出 1：待取出 2：运输中

    char sender_name[MAX_NAME_LEN];
    char receiver_name[MAX_NAME_LEN];
    char info[MAX_INFO_LEN];
    double distance;
    double volume;
    
    struct package* package_fd;//next
    // struct package* package_bk;
}package;

typedef struct HistoryRecord {
    char username[MAX_NAME_LEN];  // 用户名
    char action[100];   // 行为描述
    time_t timestamp;            // 时间戳
    struct HistoryRecord* next;  // 指向下一个记录的指针
} HistoryRecord;

typedef struct user
{
    int userid;//身份码
    int privilege;//0:管理员 1:快递员 2：用户
    int level;
    double total_cost;  // 用户累计消费金额
    char username[MAX_NAME_LEN];
    char password[MAX_PASSWD_LEN];
    char phone[12];
    char info[MAX_INFO_LEN];

    struct user* user_fd;//next
    // struct user* user_bk;
    HistoryRecord* history_head;
    package packages[MAX_INFO_LEN];//一个用户所拥有的包裹
}user;

package* Shelves[4][MAX_PACAKGE_NUM];//ABCD 四个货架

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
void postman_menu();
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
void modify_package_info(package *pkg, int attribute, int int_value, const char *str_value);
void printPackageInfo(package *current);
void addPackage(package** head);


void pick_package(package*);
void store_package();
void record_history(const char* username, const char* action);
int is_within_time_range(const char* time_str, const char* range);
void query_history(const char* username, const char* action, const char* time_range);
void find_history();
double calc_cost(package *pkg,int user_level);
void check_shelf_warning();  // 检查货架预警
void update_user_level(user* u);  // 更新用户等级

void queryPackage(package* head, int choice);
void title();
int queryPackageMenu();
#endif