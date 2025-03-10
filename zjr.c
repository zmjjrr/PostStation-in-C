#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<string.h>
#include<stdlib.h>
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

// int main()//test drive
// {
//     constructor();
    
//     package *pptr = package_head;

//     while(pptr != NULL)
//     {
//         printf("Id:%d\n",pptr->id);
//         printf("weight:%d\n",pptr->weight);
//         printf("Transport:%d\n",pptr->transport);
//         printf("Cost:%d\n",pptr->cost);
//         printf("Status:%d\n",pptr->status);
//         printf("Sender:%s\n",pptr->sender_name);
//         printf("Receiver:%s\n",pptr->receiver_name);
//         printf("Info:%s\n",pptr->info);
//         printf("-------------------------\n");
//         pptr = pptr->package_fd; 
//     }

//     printf("packages printed\n");

//     user *ptr = user_head;
//     while(ptr != NULL)
//     {
//         printf("Userid:%d\n",ptr->userid);
//         printf("Privilege:%d\n",ptr->privilege);
//         printf("Level:%d\n",ptr->level);
//         printf("Username:%s\n",ptr->username);
//         printf("Password:%s\n",ptr->password);
//         printf("Phone:%s\n",ptr->phone);
//         printf("Info:%s\n",ptr->info);
//         printf("-------------------------\n");
//         ptr = ptr->user_fd;

//     }
//     // regist();
//     // login();
//     package_ctrl();
//     destructor();
//     return 0;
// }

static int readPackageFromFile(package *ptr,FILE* fp)
{
    char buffer[0x100];
    while(fgets(buffer,sizeof(buffer),fp) != NULL)
    {
        if(strstr(buffer,"Id:") != NULL){
            sscanf(buffer,"Id:%d",&ptr->id);
        }
        else if(strstr(buffer,"weight:")!= NULL){
            sscanf(buffer,"weight:%d",&ptr->weight); 
        }
        else if(strstr(buffer,"Transport:")!= NULL){
            sscanf(buffer,"Transport:%d",&ptr->transport); 
        }
        else if(strstr(buffer,"Cost:")!= NULL){
            sscanf(buffer,"Cost:%d",&ptr->cost);
        }
        else if(strstr(buffer,"Status:")!= NULL){
            sscanf(buffer,"Status:%d",&ptr->status);
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
    fprintf(fp,"weight:%d\n",ptr->weight);
    fprintf(fp,"Transport:%d\n",ptr->transport);
    fprintf(fp,"Cost:%d\n",ptr->cost);
    fprintf(fp,"Status:%d\n",ptr->status);

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
        } 
        else if(strstr(buffer,"Privilege:")!= NULL){
            sscanf(buffer,"Privilege:%d",&ptr->privilege); 
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
    puts("Initiating packages...\n");
    FILE* package_fp = fopen("package.txt","r");
    package *package_ptr = package_head;
    package *newPackage = NULL;
    while(1)
    {
        newPackage = (struct package *)malloc(sizeof(struct package));
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
    puts("Packages initiated.\n");

    puts("Initiating users...\n");
    FILE* user_fp = fopen("user.txt","r");
    user *user_ptr = user_head;
    user *newUser = NULL;
    while(1)
    {
        newUser = (struct user *)malloc(sizeof(struct user));
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
    puts("Users initiated.\n");

}

void destructor()
{
    puts("Saving packages ...\n");
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
    puts("Packages saved.\n");

    puts("Saving users...\n");
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
    puts("Users saved.\n");
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
}