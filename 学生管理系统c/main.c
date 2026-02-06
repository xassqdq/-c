#define _CRT_SECURE_NO_WARNINGS
#include "student.h"


// 主函数
int main() {
    int choice;

    printf("欢迎使用学生管理系统！\n");

    // 加载初始数据（可选）
    // head = loadFromFile(DATA_FILE);

    while (1) {
        displayMenu();
        choice = getChoice();
        processChoice(choice);
    }

    return 0;
}