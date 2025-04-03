#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <string.h>
#include<stdlib.h>
#include <windows.h>
#include"common.h"
void title()
{
	printf("    ____                __  _____  __          __   _             \n");
	printf("   / __ \\ ____   _____ / /_/ ___/ / /_ ____ _ / /_ (_)____   ____ \n");
	printf("  / /_/ // __ \\ / ___// __/\\__ \\ / __// __ `// __// // __ \\ / __ \\\n");
	printf(" / ____// /_/ /(__  )/ /_ ___/ // /_ / /_/ // /_ / // /_/ // / / /\n");
	printf("/_/     \\____//____/ \\__//____/ \\__/ \\__,_/ \\__//_/ \\____//_/ /_/ \n");
	printf("                                                                  \n");

}

int cli()
{
	printf("%s@PostStation:$",cur_user?cur_user->username:"guest");
	char cmdBuffer[MAX_COMMAD_LEN];
	memset(cmdBuffer, 0, sizeof(cmdBuffer));
	getValidString(cmdBuffer, MAX_COMMAD_LEN - 1);


	char *token[MAX_TOKEN_LEN];
	memset(token, 0, sizeof(token));
	char* tk = strtok(cmdBuffer, " ");
	int token_num;
	for (token_num = 0;tk != NULL;++token_num)
	{
		token[token_num] = tk;
		tk = strtok(NULL, " ");
	}
	
	if (token_num == 0)
	{
		return 0;
	}

	if (!strcmp(token[0], "login"))
	{
		cur_user = login();
	}
	else if (!strcmp(token[0], "register"))
	{
		cur_user = regist();
	}
	else if (!strcmp(token[0], "exit"))
	{
		destructor();
		exit(0);
	}
	else if (!strcmp(token[0], "show"))
	{
		if (!strcmp(token[1], "packages"))
		{
			queryPackage(package_head,queryPackageMenu());
		}
		else if (!strcmp(token[1], "users"))
		{
			queryUser(user_head);
		}
	}
	else if (!strcmp(token[0], "add"))
	{
		if (!strcmp(token[1], "package"))
		{
			addPackage(&package_head);
		}
	}
	else if (!strcmp(token[0], "edit"))
	{
		if (!strcmp(token[1], "user"))
		{
			user_ctrl();
		}
		else if (!strcmp(token[1], "package"))
		{
			package_ctrl();
		}
	}
	else if (!strcmp(token[0], "interactive"))
	{
		return 1;
	}
	else if (!strcmp(token[0], "help"))
	{
		printf("login:登录\n");
		printf("register:注册\n");
		printf("interactive:进入交互界面\n");
		printf("show packages:查看包裹\n");
		printf("show users:查看用户\n");
		printf("add package:添加包裹\n");
		printf("edit user:管理用户\n");
		printf("edit package:管理包裹\n");
	}
	else
	{
		printf("未知命令！输入help查看帮助\n");
	}

	return 0;
}
