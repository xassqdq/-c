#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 定义学生信息结构体
typedef struct Student {
    char id[20];          // 学号
    char name[50];        // 姓名
    char gender[10];      // 性别
    char birthdate[20];   // 出生年月
    char major[50];       // 专业
    char class_name[50];  // 班级
    char address[100];    // 家庭地址
    char dorm[20];        // 宿舍号码
    struct Student* next; // 链表指针
} Student;

// 文件常量定义
#define DATA_FILE "students.dat"
#define BACKUP_FILE "backup.dat"
#define PASSWORD_FILE "password.dat"
#define DEFAULT_PASSWORD "admin123" // 默认密码

// 函数声明

// 主控模块函数
void displayMenu();                    // 显示主菜单
int getChoice();                       // 获取用户选择
void processChoice(int choice);        // 处理用户选择
void systemSettings();                 // 系统设置
void pauseScreen();                    // 暂停屏幕
void freeAllStudents(Student** headRef); // 释放所有学生内存

// 数据管理模块函数
// 输入功能
Student* inputStudentInfo(Student* head); // 录入单个学生信息
void addStudent(Student** head);       // 添加学生到链表
void batchInput(Student** head);       // 批量录入

// 显示功能
void displayStudent(Student* stu);     // 显示单个学生信息
void displayAllStudents(Student* head); // 显示所有学生
void displayByPage(Student* head, int pageSize); // 分页显示
void displayStudentBrief(Student* stu); // 显示简要信息

// 查找功能
Student* searchById(Student* head, char* id);   // 按学号查找
Student** searchByName(Student* head, char* name, int* count); // 按姓名查找
void displaySearchMenu(Student* head); // 查找菜单
void advancedSearch(Student* head);    // 高级查找

// 修改功能
int modifyStudent(Student* head, char* id);  // 修改学生信息
void modifyField(Student* stu);       // 修改指定字段

// 文件操作
int saveToFile(Student* head, char* filename); // 保存到文件
Student* loadFromFile(char* filename);        // 从文件加载
void backupData(Student* head);               // 数据备份
int exportToCSV(Student* head, char* filename); // 导出为CSV
Student* importFromCSV(char* filename);       // 从CSV导入

// 验证模块函数
int verifyPassword();               // 验证密码（简化版）
void setPassword();                 // 设置密码
int checkPassword();                // 检查密码（完整版）
void changePassword();              // 修改密码
void encryptPassword(char* password); // 加密密码

// 工具模块函数
void clearScreen();                 // 清屏
int stringToInt(char* str);         // 字符串转整型
void trimString(char* str);         // 去除字符串空格
int isValidDate(char* date);        // 验证日期格式
int isIdExists(Student* head, char* id); // 检查学号是否已存在
void safeInput(char* buffer, int size, const char* prompt); // 安全输入
void printSeparator();              // 输出分隔线
int inputNumber(const char* prompt, int min, int max); // 输入数字选项
Student* createStudentNode();       // 创建学生节点

#endif