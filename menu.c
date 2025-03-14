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
        printf("����ID�� %d\n", p->id);
        printf("������%d\n", p->weight);
        printf("�����ˣ�%s\n", p->sender_name);
        printf("�ռ��ˣ�%s\n", p->receiver_name);
        printf("���䷽ʽ��%d\n", p->transport);
        printf("�˷ѣ�%dԪ\n", p->cost);
        printf("��ǰ״̬��%s\n", STATUS[p->status]);
        printf("������Ϣ��%s\n", p->info);
        printf("==================================\n");
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
    printf("\t3.����������\n");//ע��
    printf("\t4.ע��\n");
    printf("***************************************\n");
    int choice = 0;
    char keystr[33];
    scanf("%d", &choice);
    while (getchar() != '\n');
    switch (choice) {
    case 1:
        package_ctrl();
        save_packages();
        break;
    case 2:
        admin_revise(user_head);
        break;
    case 3:
        keygen(keystr);//pass a pointer to the char array
        puts(keystr);
        break;
    case 4:
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
    printf("\t2.�鿴�û���Ϣ\n");
    printf("\t3.�޸��û���Ϣ\n");
    printf("\t4.ע��\n");
    printf("***************************************\n");
    int choice = 0;
    scanf("%d", &choice);
    while (getchar()!= '\n');
    switch (choice) {
    case 1:
        inform_user(cur_user);
        break;
    case 2:
        puts("under construction");
        break;
    case 3:
        user_revise(cur_user, user_head);
        break;
    case 4:
        cur_user = NULL;
        break;
    default:
        printf("��Ч���������������룡\n");
    }
}
