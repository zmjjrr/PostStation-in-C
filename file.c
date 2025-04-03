#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include"common.h"

package *package_head = NULL;
user *user_head = NULL;

static int linking_packages(user*);
static int readPackageFromFile(package* ,FILE*);
static int writePackageToFile(package *,FILE*);
static int readUserFromFile(user *,FILE* );
static int writeUserToFile(user *,FILE* );

static void CreateFile();

void constructor();
void destructor();
void save_packages();
void save_users();

static int linking_packages(user* ptr);

static int readPackageFromFile(package *ptr,FILE* fp)
{
    char buffer[0x100];
    while(fgets(buffer,sizeof(buffer),fp) != NULL)
    {
        if(strstr(buffer,"Id:") != NULL){
            sscanf(buffer,"Id:%d",&ptr->id);
            package_num++;
        }
        else if(strstr(buffer,"Weight:")!= NULL){
            sscanf(buffer,"Weight:%lf",&ptr->weight); 
        }
        else if (strstr(buffer, "Distance:") != NULL) {
            sscanf(buffer, "Distance:%lf", &ptr->distance);
        }
        else if (strstr(buffer, "Volume:") != NULL) {
            sscanf(buffer, "Volume:%lf", &ptr->volume);
        }
        else if(strstr(buffer,"Transport:")!= NULL){
            sscanf(buffer,"Transport:%d",&ptr->transport); 
        }
        else if(strstr(buffer,"Cost:")!= NULL){
            sscanf(buffer,"Cost:%lf",&ptr->cost);
        }
        else if(strstr(buffer,"Type:")!= NULL){
            sscanf(buffer,"Type:%d",&ptr->type);
        }
        else if(strstr(buffer,"Status:")!= NULL){
            sscanf(buffer,"Status:%d",&ptr->status);
        }
        else if(strstr(buffer,"T_status:")!= NULL){
            sscanf(buffer,"T_status:%d",&ptr->T_status);
        }
        else if(strstr(buffer,"Sender:")!= NULL){
            sscanf(buffer,"Sender:%s",ptr->sender_name);
        }
        else if(strstr(buffer,"Receiver:")!= NULL){
            sscanf(buffer,"Receiver:%s",ptr->receiver_name);
        }
        else if(strstr(buffer,"Info:")!= NULL){
            sscanf(buffer,"Info:%s",ptr->info); 
        }
        else if(strstr(buffer,"PACKAGE END")!= NULL){
            return 0;
        }
    }
    return -1;

    
}
static int writePackageToFile(package *ptr,FILE* fp)
{
    
    fprintf(fp,"PACKAGE BEGIN\n");
    fprintf(fp,"Id:%d\n",ptr->id);
    fprintf(fp,"Weight:%.2f\n",ptr->weight);
    fprintf(fp, "Distance:%.2f\n", ptr->distance);
    fprintf(fp, "Volume:%.2f\n", ptr->volume);
    fprintf(fp,"Transport:%d\n",ptr->transport);
    fprintf(fp,"Cost:%.2f\n",ptr->cost);
    fprintf(fp,"Type:%d\n",ptr->type);
    fprintf(fp,"Status:%d\n",ptr->status);
    fprintf(fp,"T_status:%d\n",ptr->T_status);

    fprintf(fp,"Sender:%s\n",ptr->sender_name);
    fprintf(fp,"Receiver:%s\n",ptr->receiver_name);
    fprintf(fp,"Info:%s\n",ptr->info);
    fprintf(fp,"PACKAGE END\n\n");

}
static int readUserFromFile(user *ptr,FILE* fp)
{
    char buffer[0x100];
    while(fgets(buffer,sizeof(buffer),fp)!= NULL)
    {
        if(strstr(buffer,"Userid:")!= NULL){
            sscanf(buffer,"Userid:%d",&ptr->userid);
            user_num++;
        } 
        else if(strstr(buffer,"Privilege:")!= NULL){
            sscanf(buffer,"Privilege:%d",&ptr->privilege); 
        }
        else if (strstr(buffer, "Total_cost:") != NULL) {
            sscanf(buffer, "Total_cost:%lf", &ptr->total_cost);
        }
        else if(strstr(buffer,"Level:")!= NULL){
            sscanf(buffer,"Level:%d",&ptr->level); 
        }
        else if(strstr(buffer,"Username:")!= NULL){
            sscanf(buffer,"Username:%s",ptr->username);
        }
        else if(strstr(buffer,"Password:")!= NULL){
            sscanf(buffer,"Password:%s",ptr->password);
        }
        else if(strstr(buffer,"Phone:")!= NULL){
            sscanf(buffer,"Phone:%s",ptr->phone);
        }
        else if(strstr(buffer,"Info:")!= NULL){
            sscanf(buffer,"Info:%s",ptr->info);
        }
        else if(strstr(buffer,"USER END")!= NULL){
            return 0; 
        }
    } 
    return -1;
}
static int writeUserToFile(user *ptr,FILE* fp)
{
    fprintf(fp,"USER BEGIN\n");
    fprintf(fp,"Userid:%d\n",ptr->userid);
    fprintf(fp,"Privilege:%d\n",ptr->privilege);
    fprintf(fp,"Total_cost:%lf\n", ptr->total_cost);
    fprintf(fp,"Level:%d\n",ptr->level);

    fprintf(fp,"Username:%s\n",ptr->username);
    fprintf(fp,"Password:%s\n",ptr->password); 
    fprintf(fp,"Phone:%s\n",ptr->phone);
    fprintf(fp,"Info:%s\n",ptr->info);
    fprintf(fp,"USER END\n\n");
}
void constructor()
{
    CreateFile();//create file if not exist

    // puts("Initiating packages...\n");
    FILE* package_fp = fopen("package.txt","r");
    package *package_ptr = package_head;
    package *newPackage = NULL;
    while(1)
    {
        newPackage = (struct package *)calloc(1,sizeof(struct package));
        if (newPackage == NULL) {
            perror("Memory allocation failed");
            fclose(package_fp);
            return;
        }
        if(readPackageFromFile(newPackage,package_fp) == -1)
        {
            free(newPackage);//finished reading all packages
            if (package_ptr != NULL) {
                package_ptr->package_fd = NULL;
            }
            //make sure the last fd is NULL
            break;
        }
        if(package_head == NULL)
        {
            package_head = newPackage;
            package_ptr = package_head;
        }
        else
        {
            package_ptr->package_fd = newPackage;
            package_ptr = package_ptr->package_fd;
        }
    }
    fclose(package_fp);
    // puts("Packages initiated.\n");

    // puts("Initiating users...\n");
    FILE* user_fp = fopen("user.txt","r");
    user *user_ptr = user_head;
    user *newUser = NULL;
    while(1)
    {
        newUser = (struct user *)calloc(1,sizeof(struct user));
        if(readUserFromFile(newUser,user_fp) == -1)
        {
            free(newUser);//finished reading all users
            if (user_ptr != NULL) {
                user_ptr->user_fd = NULL;
            }
            //make sure the last fd is NULL
            break;
        }
        memset(newUser->packages,0,sizeof(newUser->packages));//clear the packages array
        int num = linking_packages(newUser);//reading package info
        if(user_head == NULL)
        {
            user_head = newUser;
            user_ptr = user_head;
        }
        else
        {
            user_ptr->user_fd = newUser;
            user_ptr = user_ptr->user_fd;  
        }
    }
    fclose(user_fp);
    // puts("Users initiated.\n");

}

void destructor()
{
    save_packages();
    save_users();
}

static int linking_packages(user* user)
{
    package *package_ptr = package_head;
    int num = 0;
    while(package_ptr!= NULL)
    {
        if(strcmp(package_ptr->receiver_name,user->username) == 0)
        {
            user->packages[num] = *package_ptr;
            num++;
        }
        package_ptr = package_ptr->package_fd;
    }
    return num;
}

void CreateFile()
{
    FILE *fp = fopen("package.txt","a+");
    if (fp == NULL) {
        perror("Error opening package.txt");
        return;
    }
    fclose(fp);
    fp = fopen("user.txt","a+");
    if (fp == NULL) {
        perror("Error opening user.txt");
        return;
    }
    fclose(fp);
    fp = fopen("key.txt","a+");
    if (fp == NULL) {
        perror("Error opening key.txt");
        return;
    }
}

void save_packages()
{
    // puts("Saving packages ...\n");
    package *package_ptr = package_head;

    FILE* package_fp = fopen("package.txt","w");
    if(package_fp == NULL)
    {
        perror("Error opening file");
        return;
    }

    while(package_ptr != NULL)
    {
        writePackageToFile(package_ptr,package_fp);
        package *temp = package_ptr;
        package_ptr = package_ptr->package_fd;
    }
    fprintf(package_fp,"End Of Packages\n");
    fclose(package_fp);
    // puts("Packages saved.\n");
}

void save_users()
{
    // puts("Saving users...\n");
    user *user_ptr = user_head;

    FILE* user_fp = fopen("user.txt","w");
    if(user_fp == NULL)
    {
        perror("Error opening file");
        return;
    }

    while(user_ptr!= NULL)
    {
        writeUserToFile(user_ptr,user_fp);
        user *temp = user_ptr;
        user_ptr = user_ptr->user_fd;
    }
    fprintf(user_fp,"End Of Users\n");
    fclose(user_fp);
    // puts("Users saved.\n");
}


int keygen(char* str)
{
    srand(time(NULL));
    char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    FILE* fp = fopen("key.txt","a+");
    if(fp == NULL)
    {
        perror("Error opening file");
        return -1;
    }
    int index = 0,tmp = 0;
    char buffer[0x100];
    while(fgets(buffer,sizeof(buffer),fp) != NULL)//get max id
    {
        if(strstr(buffer,"Id:") != NULL)
        {
            sscanf(buffer,"Id:%d",&tmp);
            if(tmp > index)
            {
                index = tmp;
            }
        }
    }
    index++;

    char value[33];
    for(int i = 0;i < 32;i++)
    {
        int random_index = rand() % strlen(charset);
        value[i] = charset[random_index];
    }
    value[32] = '\0';
    fprintf(fp,"Id:%d\n",index);
    fprintf(fp,"Value:%s\n",value);
    fclose(fp);
    strcpy(str,value);
    return 0;
}
int keycheck(char* str)
{
    FILE* fp = fopen("key.txt","r");
    if(fp == NULL)
    {
        perror("Error opening file");
        return -1;
    }
    char buffer[0x100];
    int curId = 0;
    int idToRemove = -1;
    int found = 0;
    char* tempfile = (char*)calloc(1,0x21000);
    char* ptr = tempfile;
    while(fgets(buffer,sizeof(buffer),fp)!= NULL)
    {
        if (strstr(buffer,"Id:")!= NULL)
        {
            sscanf(buffer,"Id:%d",&curId);

            char value[33];
            fgets(buffer, sizeof(buffer), fp);
            sscanf(buffer,"Value:%s",value);
            if(strcmp(value,str) == 0)
            {
                idToRemove = curId;
                found = 1;
                continue;
            }
            if (idToRemove != -1 && curId > idToRemove) 
            {
                // 更新 ID
                sprintf(buffer, "Id:%d\n", curId - 1);
                strcat(ptr, buffer);
            } 
            else
            {
                sprintf(buffer, "Id:%d\n", curId);
                strcat(ptr, buffer);
            }
            sprintf(buffer, "Value:%s\n", value);
            strcat(ptr, buffer);
        }
    }
    fclose(fp);

    if (found)
    {
        fp = fopen("key.txt", "w");
        if (fp == NULL)
        {
            perror("Error opening file");
            free(tempfile);
            return -1;
        }
        fputs(tempfile, fp);
        fclose(fp);
        free(tempfile);
    }
    else
    {
        free(tempfile);
    }
    return found;
}


