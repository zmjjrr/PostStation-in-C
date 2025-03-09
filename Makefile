# 定义编译器和编译选项
CC = gcc
CFLAGS = -Wall -g

# 源文件和目标文件列表
SRCS = main.c login.c pick_package_inform.c reg.c xyh.c zjr.c
OBJS = $(SRCS:.c=.o)

# 目标：生成可执行文件
myprogram: $(OBJS)
    $(CC) $(CFLAGS) $(OBJS) -o myprogram

# 模式规则：生成目标文件
%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

# 清理规则：删除生成的文件
clean:
    rm -f myprogram $(OBJS)