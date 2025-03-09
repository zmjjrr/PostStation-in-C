#include<stdio.h>
#include"common.h"


user* cur_user = NULL;
int main()
{
    constructor();
    while(1)
    {
        print_menu();
    }
    destructor();
}