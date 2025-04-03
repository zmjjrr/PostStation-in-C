#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"common.h"


void default_menu() {
    printf("***************************************\n");
    printf("\t��ӭʹ�ò�����վϵͳ\n");
    printf("\t1.��¼\n");
    printf("\t2.ע��\n");
    printf("\t3.���沢�˳�\n");
    printf("\t4.���������н���\n");
    printf("***************************************\n");
}

//�û�����֪ͨ
void inform_user(user* u) {
    if (u == NULL) {
        printf("���� �û�ָ��Ϊ��\n");
        return;
    }
    const char* STATUS[] = { "��","����","��ʧ" };
    int have_package = 0;
    printf("**�û�����֪ͨ**\n");
    for (int i = 0; i < MAX_INFO_LEN; i++) {
        if (u->packages[i].id == 0)
            break;
        have_package = 1;
        package* p = &(u->packages[i]);
        printPackageInfo(p);
    }
    if (!have_package) {
        printf("����ǰû�п�ݼ�¼\n");
    }
    printf("**********************************\n");
}

//����Ա����
void admin_menu() {
    printf("***************************************\n");
    printf("\t����Ա�˵�\n");
    printf("\t1.�������\n");
    printf("\t2.�����û�\n");
    printf("\t3.��ѯ��ʷ��¼\n");
    printf("\t4.����������\n");//ע��
    printf("\t5.ע��\n");
    printf("***************************************\n");
    int choice = 0;
    char keystr[33];
    choice = getValidInt(1, 5);
    switch (choice) {
    case 1:
        package_ctrl();
        record_history(cur_user->username, "�������");
        break;
    case 2:
        user_ctrl();
        save_users();
        record_history(cur_user->username, "�����û�");
        break;
    case 3:
        find_history();
        break;
    case 4:
        keygen(keystr);//pass a pointer to the char array
        puts(keystr);
        record_history(cur_user->username, "����������");
        break;
    case 5:
        cur_user = NULL;
        break;
    default:
        printf("��Ч���������������룡\n");
    }
}

//�û���¼��˵�
void user_menu() {
    printf("***************************************\n");
    printf("\t��ӭ�û�%s\n", cur_user->username);
    printf("\t1.�鿴����\n");
    printf("\t2 �ĳ�����\n");
    printf("\t3.�鿴�û���Ϣ\n");
    printf("\t4.�޸��û���Ϣ\n");
    printf("\t5.ע��\n");
    printf("***************************************\n");
    int choice = 0;
    choice = getValidInt(1, 5);
    switch (choice) {
    case 1:
        inform_user(cur_user);
        record_history(cur_user->username, "��ѯ����");
        break;
    case 2:
        addPackage(&package_head);
        record_history(cur_user->username, "�ĳ�����");
        save_packages();
        break;
    case 3:
        printUserInfo(cur_user);
        break;
    case 4:
        user_revise(cur_user, user_head);
        record_history(cur_user->username, "�޸���Ϣ");
        save_users();
        break;
    case 5:
        cur_user = NULL;
        break;
    default:
        printf("��Ч���������������룡\n");
    }
}
             
int user_ctrl() {
    int choice;

    do {
        printf("\n=== �û�����ϵͳ ===\n");
        printf("1. ��ѯ�û�\n");
        printf("2. ɾ���û�\n");
        printf("3. �޸��û���Ϣ\n");
        printf("4. �˳�\n");
        printf("���������ѡ��: ");
        choice = getValidInt(1, 99);

        switch (choice) {
        case 1:
            queryUser(user_head);
            record_history(cur_user->username, "��ѯ�û�");
            break;
        case 2:
            deleteUser(&user_head);
            record_history(cur_user->username, "ɾ���û�");
            break;
        case 3:
            admin_revise(user_head);
            record_history(cur_user->username, "�޸���Ϣ");
            break;
        case 4:
            printf("�˳�ϵͳ��\n");
            break;
        default:
            printf("��Ч��ѡ�����������룡\n");
        }
    } while (choice != 4);

    return 0;
}

// ���Ա����
void postman_menu() {
    printf("***************************************\n");
    printf("\t���Ա�˵�\n");
    printf("\t1. ȡ��\n");
    printf("\t2. ���\n");
    printf("\t3. ��ѯ\n");
    printf("\t4. �鿴�û���Ϣ\n");
    printf("\t5. �޸��û���Ϣ\n");
    printf("\t6. �˳���¼\n");
    printf("***************************************\n");

    int choice = 0;
    int found = 0;
    int id = 0;
    choice = getValidInt(1, 6);
    char sender[MAX_NAME_LEN];
    package* current = package_head;
    strcpy(sender, cur_user->username);

    switch (choice) {
    case 1:
        printf("���������id:");
        id = getValidInt(1, package_num);
        package* p = package_head;
        while (p != NULL)
        {
            if (p->id == id)
            {
                pick_package(p);
                break;
            }
            p = p->package_fd;
        }
        if (p == NULL)
        {
            printf("δ�ҵ��ð���\n");
        }
        save_packages();
        record_history(cur_user->username, "ȡ��");
        break;
    case 2:
        store_package();
        save_packages();
        record_history(cur_user->username, "���");
        break;
    case 3:
        queryPackage(package_head,queryPackageMenu());
        record_history(cur_user->username, "��ѯ����");
        break;
    case 4:
        printUserInfo(cur_user);
        record_history(cur_user->username, "��ѯ�û�");
        break;
    case 5:
        user_revise(cur_user,user_head);
        break;
    case 6:
        cur_user = NULL;
        break;
    default:
        printf("��Ч���������������룡\n");
    }
}