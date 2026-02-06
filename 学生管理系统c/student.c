#define _CRT_SECURE_NO_WARNINGS
#include "student.h"

// 全局变量定义
Student* head = NULL;  // 链表头指针
int studentCount = 0;  // 学生总数
const char* password = "admin123";  // 默认密码

//函数实现
//主控模块函数
void displayMenu() {
    printf("========================================\n");
    printf("       学生管理系统 v1.0              \n");
    printf("========================================\n");
    printf("      1. 录入学生信息                \n");
    printf("      2. 显示所有学生                \n");
    printf("      3. 查找学生信息                \n");
    printf("      4. 修改学生信息                \n");
    printf("      5. 保存数据到文件              \n");
    printf("      6. 从文件加载数据              \n");
    printf("      7. 系统设置                    \n");
    printf("      0. 退出系统                    \n");
    printf("========================================\n");
    printf("当前学生总数: %d\n", studentCount);
    printf("========================================\n");
    printf("请输入您的选择(0-7): ");

}// 显示主菜单

int getChoice() {
    int choice;
    char input[20];

    while (1) {
        fgets(input, sizeof(input), stdin);

		// 移除换行符，配合fgets使用
        input[strcspn(input, "\n")] = '\0';

        // 检查输入是否为空
        if (strlen(input) == 0) {
            printf("请输入有效选择(0-7): ");
            continue;
        }

        // 检查是否为数字
		int isValid = 1;//校验标志，默认为有效
        for (int i = 0; i < strlen(input); i++) {
            if (!isdigit(input[i])) {
                isValid = 0;
                break;
            }
        }

        if (!isValid) {
            printf("请输入数字(0-7): ");
            continue;
        }

        choice = atoi(input);

        // 检查范围
        if (choice >= 0 && choice <= 7) {
            return choice;
        }
        else {
            printf("输入无效，请输入0-7之间的数字: ");
        }
    }
}// 获取用户选择

// 处理用户选择
void processChoice(int choice) {
    switch (choice) {
    case 1:
        addStudent(&head);
        studentCount++;
        printf("录入成功！\n");
        break;

    case 2:
        if (studentCount == 0) {
            printf("当前没有学生信息，请先录入数据。\n");
        }
        else {
            displayAllStudents(head);
        }
        break;

    case 3:
        if (studentCount == 0) {
            printf("当前没有学生信息，请先录入数据。\n");
        }
        else {
            displaySearchMenu(head);
        }
        break;

    case 4:
        if (studentCount == 0) {
            printf("当前没有学生信息，请先录入数据。\n");
        }
        else {
            char id[20];
            printf("请输入要修改的学生学号: ");
            fgets(id, sizeof(id), stdin);
            id[strcspn(id, "\n")] = '\0';

            // 密码验证
            if (verifyPassword()) {
                if (modifyStudent(head, id)) {
                    printf("修改成功！\n");
                }
                else {
                    printf("未找到该学号的学生。\n");
                }
            }
            else {
                printf("密码错误，修改失败！\n");
            }
        }
        break;

    case 5:
        if (studentCount == 0) {
            printf("当前没有学生信息，无法保存。\n");
        }
        else {
            if (saveToFile(head, DATA_FILE)) {
                printf("数据保存成功！\n");
            }
            else {
                printf("数据保存失败！\n");
            }
        }
        break;

    case 6:
    {
        Student* loadedHead = loadFromFile(DATA_FILE);
        if (loadedHead != NULL) {
            // 释放原有内存
            freeAllStudents(&head);
            head = loadedHead;

            // 重新计算学生数量
            studentCount = 0;
            Student* current = head;
            while (current != NULL) {
                studentCount++;
                current = current->next;
            }

            printf("数据加载成功！共加载 %d 条记录。\n", studentCount);
        }
        else {
            printf("数据加载失败或文件不存在。\n");
        }
    }
    break;

    case 7:
        systemSettings();
        break;

    case 0:
        printf("正在退出系统...\n");

        // 询问是否保存数据
        if (studentCount > 0) {
            printf("是否保存当前数据到文件？(y/n): ");
            char saveChoice;
            scanf(" %c", &saveChoice);
            getchar();  // 清除输入缓冲区

            if (saveChoice == 'y' || saveChoice == 'Y') {
                saveToFile(head, DATA_FILE);
                printf("数据已保存。\n");
            }
        }

        // 释放链表内存
        freeAllStudents(&head);
        printf("系统已退出，感谢使用！\n");
        exit(0);
        break;

    default:
        printf("无效选择，请重新输入。\n");
        break;
    }

    // 暂停屏幕，等待用户按键继续
    pauseScreen();
}

// 辅助函数：释放所有学生内存
void freeAllStudents(Student** headRef) {
    Student* current = *headRef;
    Student* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *headRef = NULL;
}

// 系统设置
void systemSettings() {
    int settingChoice;

    do {
        printf("========================================\n");
        printf("           系统设置                    \n");
        printf("========================================\n");
        printf("      1. 修改系统密码                \n");
        printf("      2. 数据备份                  \n");
        printf("      3. 清空所有数据                \n");
        printf("      0. 返回主菜单                  \n");
        printf("========================================\n");
        printf("请输入您的选择: ");

        char input[10];
        fgets(input, sizeof(input), stdin);
        settingChoice = atoi(input);

        switch (settingChoice) {
        case 1:
            changePassword();
            break;

        case 2:
            backupData(head);
            break;

        case 3:
        {
            printf("警告：此操作将清空所有数据，且不可恢复！\n");
            printf("确认清空所有数据吗？(y/n): ");
            char confirm;
            scanf(" %c", &confirm);
            getchar();

            if (confirm == 'y' || confirm == 'Y') {
                freeAllStudents(&head);
                studentCount = 0;
                printf("所有数据已清空。\n");
            }
            else {
                printf("操作已取消。\n");
            }
        }
        break;

        case 0:
            printf("返回主菜单。\n");
            break;

        default:
            printf("无效选择，请重新输入。\n");
            break;
        }

        if (settingChoice != 0) {
            pauseScreen();
        }

    } while (settingChoice != 0);
}



// 清屏
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 暂停屏幕
void pauseScreen() {
    printf("\n按任意键继续...");
    getchar(); // 等待用户按键
}

// 字符串转整型
int stringToInt(char* str) {
    int result = 0;
    int sign = 1;
    int i = 0;

    // 处理负号
    if (str[0] == '-') {
        sign = -1;
        i = 1;
    }

    // 转换数字
    while (str[i] != '\0') {
        if (isdigit(str[i])) {
            result = result * 10 + (str[i] - '0');
        }
        else {
            // 非数字字符，返回0或错误处理
            return 0;
        }
        i++;
    }

    return result * sign;
}

// 去除字符串首尾空格
void trimString(char* str) {
    int i = 0, j = 0;
    int len = strlen(str);

    // 去除头部空格
    while (i < len && isspace((unsigned char)str[i])) {
        i++;
    }

    // 复制非空格字符
    while (i < len) {
        str[j++] = str[i++];
    }

    // 字符串结束
    str[j] = '\0';

    // 去除尾部空格
    j = strlen(str) - 1;
    while (j >= 0 && isspace((unsigned char)str[j])) {
        str[j] = '\0';
        j--;
    }
}

// 验证日期格式（YYYY-MM）
int isValidDate(char* date) {
    if (strlen(date) != 7) {
        return 0;
    }

    // 检查格式 YYYY-MM
    for (int i = 0; i < 7; i++) {
        if (i == 4) {
            if (date[i] != '-') return 0;
        }
        else {
            if (!isdigit(date[i])) return 0;
        }
    }

    // 提取年份和月份
    char yearStr[5], monthStr[3];
    strncpy(yearStr, date, 4);
    yearStr[4] = '\0';
    strncpy(monthStr, date + 5, 2);
    monthStr[2] = '\0';

    int year = atoi(yearStr);
    int month = atoi(monthStr);

    // 检查月份范围
    if (month < 1 || month > 12) {
        return 0;
    }

    // 检查年份范围（假设学生年龄在15-30岁之间）
    int currentYear = 2024; // 可以改为动态获取当前年份
    if (year < currentYear - 30 || year > currentYear - 15) {
        printf("提示：出生年份似乎不合理。\n");
        return 0;
    }

    return 1;
}

// 检查学号是否已存在
int isIdExists(Student* head, char* id) {
    Student* current = head;
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// 获取安全的字符串输入
void safeInput(char* buffer, int size, const char* prompt) {
    printf("%s", prompt);

    while (1) {
        fgets(buffer, size, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        trimString(buffer);

        if (strlen(buffer) == 0) {
            printf("输入不能为空，请重新输入: ");
            continue;
        }
        break;
    }
}

// 格式化输出分隔线
void printSeparator() {
    printf("------------------------------------------------------------\n");
}

// 输入数字选项
int inputNumber(const char* prompt, int min, int max) {
    int choice;
    char input[20];

    while (1) {
        printf("%s", prompt);
        fgets(input, sizeof(input), stdin);

        // 移除换行符
        input[strcspn(input, "\n")] = '\0';

        // 检查是否为数字
        int isValid = 1;
        for (int i = 0; i < strlen(input); i++) {
            if (!isdigit(input[i])) {
                isValid = 0;
                break;
            }
        }

        if (!isValid) {
            printf("请输入有效的数字！\n");
            continue;
        }

        choice = atoi(input);

        if (choice >= min && choice <= max) {
            return choice;
        }
        else {
            printf("输入无效，请输入 %d-%d 之间的数字: ", min, max);
        }
    }
}

// 创建新的学生节点
Student* createStudentNode() {
    Student* newStudent = (Student*)malloc(sizeof(Student));
    if (newStudent == NULL) {
        printf("内存分配失败！\n");
        return NULL;
    }

    // 初始化指针
    newStudent->next = NULL;
    return newStudent;
}

// 录入单个学生信息
Student* inputStudentInfo(Student* head) {
    Student* newStudent = createStudentNode();
    if (newStudent == NULL) {
        return head;
    }

    printf("\n======== 录入学生信息 ========\n");
    printf("请输入学生信息（输入'q'返回上一级）\n\n");

    // 输入学号
    char id[20];
    while (1) {
        safeInput(id, sizeof(id), "学号: ");

        if (strcmp(id, "q") == 0) {
            free(newStudent);
            return head;
        }

        // 检查学号是否已存在
        if (isIdExists(head, id)) {
            printf("学号已存在，请重新输入！\n");
            continue;
        }

        // 检查学号格式
        if (strlen(id) != 8) {
            printf("学号应为8位数字，请重新输入！\n");
            continue;
        }

        // 验证学号是否为数字
        int valid = 1;
        for (int i = 0; i < 8; i++) {
            if (!isdigit(id[i])) {
                valid = 0;
                break;
            }
        }

        if (!valid) {
            printf("学号应全为数字，请重新输入！\n");
            continue;
        }

        strcpy(newStudent->id, id);
        break;
    }

    // 输入姓名
    safeInput(newStudent->name, sizeof(newStudent->name), "姓名: ");
    if (strcmp(newStudent->name, "q") == 0) {
        free(newStudent);
        return head;
    }

    // 输入性别
    while (1) {
        char gender[10];
        safeInput(gender, sizeof(gender), "性别(男/女): ");

        if (strcmp(gender, "q") == 0) {
            free(newStudent);
            return head;
        }

        if (strcmp(gender, "男") == 0 || strcmp(gender, "女") == 0) {
            strcpy(newStudent->gender, gender);
            break;
        }
        else {
            printf("性别输入错误，请输入'男'或'女'！\n");
        }
    }

    // 输入出生年月
    while (1) {
        char birthdate[20];
        safeInput(birthdate, sizeof(birthdate), "出生年月(YYYY-MM): ");

        if (strcmp(birthdate, "q") == 0) {
            free(newStudent);
            return head;
        }

        if (isValidDate(birthdate)) {
            strcpy(newStudent->birthdate, birthdate);
            break;
        }
        else {
            printf("日期格式错误，请使用YYYY-MM格式！\n");
        }
    }

    // 输入专业
    safeInput(newStudent->major, sizeof(newStudent->major), "专业: ");
    if (strcmp(newStudent->major, "q") == 0) {
        free(newStudent);
        return head;
    }

    // 输入班级
    safeInput(newStudent->class_name, sizeof(newStudent->class_name), "班级: ");
    if (strcmp(newStudent->class_name, "q") == 0) {
        free(newStudent);
        return head;
    }

    // 输入家庭地址
    safeInput(newStudent->address, sizeof(newStudent->address), "家庭地址: ");
    if (strcmp(newStudent->address, "q") == 0) {
        free(newStudent);
        return head;
    }

    // 输入宿舍号码
    safeInput(newStudent->dorm, sizeof(newStudent->dorm), "宿舍号码: ");
    if (strcmp(newStudent->dorm, "q") == 0) {
        free(newStudent);
        return head;
    }

    return newStudent;
}

// 添加学生到链表
void addStudent(Student** head) {
    clearScreen();

    int count;
    char input[20];

    printf("======== 学生信息录入 ========\n");
    printf("1. 单条录入\n");
    printf("2. 批量录入\n");
    printf("0. 返回主菜单\n");
    printSeparator();

    int choice = inputNumber("请选择录入方式: ", 0, 2);

    switch (choice) {
    case 1:
    {
        Student* newStudent = inputStudentInfo(*head);
        if (newStudent != NULL) {
            // 插入到链表头部
            newStudent->next = *head;
            *head = newStudent;
            printf("\n学生信息录入成功！\n");
        }
    }
    break;

    case 2:
        batchInput(head);
        break;

    case 0:
        return;
    }
}

// 批量录入
void batchInput(Student** head) {
    int count;
    char input[20];

    printf("\n======== 批量录入 ========\n");
    printf("请输入要录入的学生数量(1-100): ");

    while (1) {
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (!isdigit(input[0])) {
            printf("请输入有效的数字: ");
            continue;
        }

        count = atoi(input);
        if (count > 0 && count <= 100) {
            break;
        }
        else {
            printf("请输入1-100之间的数字: ");
        }
    }

    int successCount = 0;
    for (int i = 0; i < count; i++) {
        printf("\n--- 录入第 %d 个学生 ---\n", i + 1);
        Student* newStudent = inputStudentInfo(*head);

        if (newStudent != NULL) {
            // 插入到链表头部
            newStudent->next = *head;
            *head = newStudent;
            successCount++;
        }
        else {
            printf("第 %d 个学生录入失败或已取消。\n", i + 1);
            break;
        }
    }

    printf("\n批量录入完成！成功录入 %d 个学生。\n", successCount);
}

// 显示单个学生信息
void displayStudent(Student* stu) {
    if (stu == NULL) {
        printf("学生信息不存在！\n");
        return;
    }

    printSeparator();
    printf("学号: %s\n", stu->id);
    printf("姓名: %s\n", stu->name);
    printf("性别: %s\n", stu->gender);
    printf("出生年月: %s\n", stu->birthdate);
    printf("专业: %s\n", stu->major);
    printf("班级: %s\n", stu->class_name);
    printf("家庭地址: %s\n", stu->address);
    printf("宿舍号码: %s\n", stu->dorm);
    printSeparator();
}

// 显示所有学生
void displayAllStudents(Student* head) {
    clearScreen();

    if (head == NULL) {
        printf("当前没有学生信息！\n");
        return;
    }

    Student* current = head;
    int count = 0;

    printf("======== 所有学生信息 ========\n");
    printf("学生总数: ");

    // 先统计总数
    while (current != NULL) {
        count++;
        current = current->next;
    }

    printf("%d\n", count);
    printSeparator();

    // 显示方式选择
    printf("1. 全部显示\n");
    printf("2. 分页显示（每页10条）\n");
    printf("0. 返回主菜单\n");
    printSeparator();

    int choice = inputNumber("请选择显示方式: ", 0, 2);

    if (choice == 0) {
        return;
    }
    else if (choice == 1) {
        // 全部显示
        current = head;
        int index = 1;
        while (current != NULL) {
            printf("【第 %d 个学生】\n", index);
            displayStudent(current);
            index++;
            current = current->next;
        }
    }
    else if (choice == 2) {
        // 分页显示
        displayByPage(head, 10);
    }
}

// 分页显示
void displayByPage(Student* head, int pageSize) {
    if (head == NULL) {
        printf("没有学生信息可显示！\n");
        return;
    }

    Student* current = head;
    int totalCount = 0;
    int page = 1;

    // 统计总数
    while (current != NULL) {
        totalCount++;
        current = current->next;
    }

    int totalPages = (totalCount + pageSize - 1) / pageSize;

    while (1) {
        clearScreen();
        printf("======== 学生信息 - 第 %d/%d 页 ========\n", page, totalPages);
        printf("学生总数: %d\n", totalCount);
        printSeparator();

        // 计算当前页起始和结束位置
        int start = (page - 1) * pageSize + 1;
        int end = page * pageSize;
        if (end > totalCount) end = totalCount;

        // 显示当前页
        current = head;
        int index = 1;
        int displayIndex = 1;

        // 移动到当前页起始位置
        while (current != NULL && index < start) {
            current = current->next;
            index++;
        }

        // 显示当前页的学生
        while (current != NULL && index <= end) {
            printf("【第 %d 个学生】\n", start + displayIndex - 1);
            displayStudent(current);
            current = current->next;
            index++;
            displayIndex++;
        }

        printf("\n");
        printf("1. 上一页\n");
        printf("2. 下一页\n");
        printf("3. 跳转到指定页\n");
        printf("0. 返回\n");
        printSeparator();

        int choice = inputNumber("请选择操作: ", 0, 3);

        switch (choice) {
        case 1: // 上一页
            if (page > 1) {
                page--;
            }
            else {
                printf("已经是第一页了！\n");
                pauseScreen();
            }
            break;

        case 2: // 下一页
            if (page < totalPages) {
                page++;
            }
            else {
                printf("已经是最后一页了！\n");
                pauseScreen();
            }
            break;

        case 3: // 跳转
        {
            int target = inputNumber("请输入要跳转的页数: ", 1, totalPages);
            page = target;
        }
        break;

        case 0: // 返回
            return;
        }
    }
}

// 显示简要信息（用于列表）
void displayStudentBrief(Student* stu) {
    if (stu == NULL) return;
    printf("%-10s %-15s %-5s %-10s %-20s\n",
        stu->id, stu->name, stu->gender,
        stu->birthdate, stu->major);
}


// 按学号查找（顺序查找）
Student* searchById(Student* head, char* id) {
    Student* current = head;

    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

// 按姓名查找（支持部分匹配）
Student** searchByName(Student* head, char* name, int* count) {
    Student** results = NULL;
    *count = 0;
    Student* current = head;

    // 第一次遍历，统计匹配的数量
    while (current != NULL) {
        if (strstr(current->name, name) != NULL) {
            (*count)++;
        }
        current = current->next;
    }

    if (*count == 0) {
        return NULL;
    }

    // 分配内存存储结果指针
    results = (Student**)malloc(sizeof(Student*) * (*count));
    if (results == NULL) {
        printf("内存分配失败！\n");
        *count = 0;
        return NULL;
    }

    // 第二次遍历，存储匹配的指针
    current = head;
    int index = 0;
    while (current != NULL) {
        if (strstr(current->name, name) != NULL) {
            results[index++] = current;
        }
        current = current->next;
    }

    return results;
}

// 显示查找菜单
void displaySearchMenu(Student* head) {
    clearScreen();

    while (1) {
        printf("======== 学生信息查找 ========\n");
        printf("1. 按学号查找\n");
        printf("2. 按姓名查找\n");
        printf("3. 高级查找\n");
        printf("0. 返回主菜单\n");
        printSeparator();

        int choice = inputNumber("请选择查找方式: ", 0, 3);

        switch (choice) {
        case 1: // 按学号查找
        {
            char id[20];
            safeInput(id, sizeof(id), "请输入学号: ");

            Student* result = searchById(head, id);
            if (result != NULL) {
                printf("\n查找结果：\n");
                displayStudent(result);
            }
            else {
                printf("未找到学号为 %s 的学生！\n", id);
            }
            pauseScreen();
        }
        break;

        case 2: // 按姓名查找
        {
            char name[50];
            safeInput(name, sizeof(name), "请输入姓名(支持模糊匹配): ");

            int count = 0;
            Student** results = searchByName(head, name, &count);

            if (results != NULL && count > 0) {
                printf("\n找到 %d 个匹配的学生：\n", count);
                for (int i = 0; i < count; i++) {
                    printf("\n【第 %d 个匹配结果】\n", i + 1);
                    displayStudent(results[i]);
                }
                free(results);
            }
            else {
                printf("未找到姓名为 %s 的学生！\n", name);
            }
            pauseScreen();
        }
        break;

        case 3: // 高级查找
            advancedSearch(head);
            break;

        case 0: // 返回
            return;
        }
    }
}

// 高级查找
void advancedSearch(Student* head) {
    clearScreen();
    printf("======== 高级查找 ========\n");
    printf("1. 按专业查找\n");
    printf("2. 按班级查找\n");
    printf("3. 按性别查找\n");
    printf("0. 返回\n");
    printSeparator();

    int choice = inputNumber("请选择查找条件: ", 0, 3);

    if (choice == 0) return;

    char keyword[50];
    Student* current = head;
    int found = 0;

    switch (choice) {
    case 1: // 按专业查找
        safeInput(keyword, sizeof(keyword), "请输入专业: ");
        printf("\n查找结果：\n");
        while (current != NULL) {
            if (strstr(current->major, keyword) != NULL) {
                displayStudent(current);
                found = 1;
            }
            current = current->next;
        }
        break;

    case 2: // 按班级查找
        safeInput(keyword, sizeof(keyword), "请输入班级: ");
        printf("\n查找结果：\n");
        while (current != NULL) {
            if (strstr(current->class_name, keyword) != NULL) {
                displayStudent(current);
                found = 1;
            }
            current = current->next;
        }
        break;

    case 3: // 按性别查找
        safeInput(keyword, sizeof(keyword), "请输入性别(男/女): ");
        printf("\n查找结果：\n");
        while (current != NULL) {
            if (strcmp(current->gender, keyword) == 0) {
                displayStudent(current);
                found = 1;
            }
            current = current->next;
        }
        break;
    }

    if (!found) {
        printf("未找到匹配的记录！\n");
    }

    pauseScreen();
}



// 修改指定字段
void modifyField(Student* stu) {
    if (stu == NULL) {
        printf("学生信息不存在！\n");
        return;
    }

    while (1) {
        printf("\n======== 修改字段 ========\n");
        printf("当前学生：%s - %s\n", stu->id, stu->name);
        printf("1. 姓名\n");
        printf("2. 性别\n");
        printf("3. 出生年月\n");
        printf("4. 专业\n");
        printf("5. 班级\n");
        printf("6. 家庭地址\n");
        printf("7. 宿舍号码\n");
        printf("8. 修改所有字段\n");
        printf("0. 完成修改\n");
        printSeparator();

        int choice = inputNumber("请选择要修改的字段: ", 0, 8);

        if (choice == 0) {
            printf("修改完成！\n");
            break;
        }

        switch (choice) {
        case 1: // 姓名
            safeInput(stu->name, sizeof(stu->name), "请输入新姓名: ");
            printf("姓名已修改为: %s\n", stu->name);
            break;

        case 2: // 性别
            while (1) {
                char gender[10];
                safeInput(gender, sizeof(gender), "请输入性别(男/女): ");
                if (strcmp(gender, "男") == 0 || strcmp(gender, "女") == 0) {
                    strcpy(stu->gender, gender);
                    printf("性别已修改为: %s\n", stu->gender);
                    break;
                }
                else {
                    printf("输入错误，请输入'男'或'女'！\n");
                }
            }
            break;

        case 3: // 出生年月
            while (1) {
                char birthdate[20];
                safeInput(birthdate, sizeof(birthdate), "请输入出生年月(YYYY-MM): ");
                if (isValidDate(birthdate)) {
                    strcpy(stu->birthdate, birthdate);
                    printf("出生年月已修改为: %s\n", stu->birthdate);
                    break;
                }
                else {
                    printf("日期格式错误，请使用YYYY-MM格式！\n");
                }
            }
            break;

        case 4: // 专业
            safeInput(stu->major, sizeof(stu->major), "请输入新专业: ");
            printf("专业已修改为: %s\n", stu->major);
            break;

        case 5: // 班级
            safeInput(stu->class_name, sizeof(stu->class_name), "请输入新班级: ");
            printf("班级已修改为: %s\n", stu->class_name);
            break;

        case 6: // 家庭地址
            safeInput(stu->address, sizeof(stu->address), "请输入新家庭地址: ");
            printf("家庭地址已修改为: %s\n", stu->address);
            break;

        case 7: // 宿舍号码
            safeInput(stu->dorm, sizeof(stu->dorm), "请输入新宿舍号码: ");
            printf("宿舍号码已修改为: %s\n", stu->dorm);
            break;

        case 8: // 修改所有字段
            printf("=== 重新输入所有信息 ===\n");
            safeInput(stu->name, sizeof(stu->name), "姓名: ");

            while (1) {
                char gender[10];
                safeInput(gender, sizeof(gender), "性别(男/女): ");
                if (strcmp(gender, "男") == 0 || strcmp(gender, "女") == 0) {
                    strcpy(stu->gender, gender);
                    break;
                }
                else {
                    printf("输入错误，请输入'男'或'女'！\n");
                }
            }

            while (1) {
                char birthdate[20];
                safeInput(birthdate, sizeof(birthdate), "出生年月(YYYY-MM): ");
                if (isValidDate(birthdate)) {
                    strcpy(stu->birthdate, birthdate);
                    break;
                }
                else {
                    printf("日期格式错误，请使用YYYY-MM格式！\n");
                }
            }

            safeInput(stu->major, sizeof(stu->major), "专业: ");
            safeInput(stu->class_name, sizeof(stu->class_name), "班级: ");
            safeInput(stu->address, sizeof(stu->address), "家庭地址: ");
            safeInput(stu->dorm, sizeof(stu->dorm), "宿舍号码: ");
            printf("所有信息已更新！\n");
            break;
        }

        // 显示修改后的信息
        printf("\n修改后的学生信息：\n");
        displayStudent(stu);

        // 询问是否继续修改
        printf("\n是否继续修改其他字段？(y/n): ");
        char continueChoice;
        scanf(" %c", &continueChoice);
        getchar(); // 清除缓冲区

        if (continueChoice != 'y' && continueChoice != 'Y') {
            break;
        }
    }
}

// 修改学生信息
int modifyStudent(Student* head, char* id) {
    Student* target = searchById(head, id);

    if (target == NULL) {
        return 0;
    }

    clearScreen();
    printf("======== 修改学生信息 ========\n");
    printf("当前学生信息：\n");
    displayStudent(target);

    printf("\n");
    modifyField(target);

    return 1;
}

// 保存到文件
int saveToFile(Student* head, char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法打开文件 %s 进行写入！\n", filename);
        return 0;
    }

    Student* current = head;
    int count = 0;

    while (current != NULL) {
        fprintf(file, "%s|%s|%s|%s|%s|%s|%s|%s\n",
            current->id,
            current->name,
            current->gender,
            current->birthdate,
            current->major,
            current->class_name,
            current->address,
            current->dorm);
        count++;
        current = current->next;
    }

    fclose(file);
    printf("成功保存 %d 条记录到文件 %s\n", count, filename);
    return 1;
}

// 从文件加载
Student* loadFromFile(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件 %s 或文件不存在！\n", filename);
        return NULL;
    }

    Student* head = NULL;
    Student* tail = NULL;
    char line[256];
    int count = 0;

    printf("正在加载数据...\n");

    while (fgets(line, sizeof(line), file)) {
        // 移除换行符
        line[strcspn(line, "\n")] = '\0';

        // 分割字符串
        char* token = strtok(line, "|");
        if (token == NULL) continue;

        // 创建新节点
        Student* newStudent = (Student*)malloc(sizeof(Student));
        if (newStudent == NULL) {
            printf("内存分配失败！\n");
            fclose(file);
            return head;
        }

        // 学号
        strcpy(newStudent->id, token);

        // 姓名
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(newStudent->name, token);

        // 性别
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(newStudent->gender, token);

        // 出生年月
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(newStudent->birthdate, token);

        // 专业
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(newStudent->major, token);

        // 班级
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(newStudent->class_name, token);

        // 家庭地址
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(newStudent->address, token);

        // 宿舍号码
        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strcpy(newStudent->dorm, token);

        newStudent->next = NULL;
        count++;

        // 添加到链表
        if (head == NULL) {
            head = newStudent;
            tail = newStudent;
        }
        else {
            tail->next = newStudent;
            tail = newStudent;
        }
    }

    fclose(file);
    printf("从文件 %s 加载了 %d 条记录\n", filename, count);
    return head;
}

// 数据备份
void backupData(Student* head) {
    if (head == NULL) {
        printf("没有数据可以备份！\n");
        return;
    }

    char backupName[100];
    printf("请输入备份文件名(默认: backup.dat): ");
    fgets(backupName, sizeof(backupName), stdin);
    backupName[strcspn(backupName, "\n")] = '\0';

    if (strlen(backupName) == 0) {
        strcpy(backupName, "backup.dat");
    }

    if (saveToFile(head, backupName)) {
        printf("数据备份成功！备份文件: %s\n", backupName);
    }
    else {
        printf("数据备份失败！\n");
    }

    pauseScreen();
}

// 导出为CSV格式
int exportToCSV(Student* head, char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法打开文件 %s 进行写入！\n", filename);
        return 0;
    }

    // 写入CSV标题
    fprintf(file, "学号,姓名,性别,出生年月,专业,班级,家庭地址,宿舍号码\n");

    Student* current = head;
    int count = 0;

    while (current != NULL) {
        fprintf(file, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
            current->id,
            current->name,
            current->gender,
            current->birthdate,
            current->major,
            current->class_name,
            current->address,
            current->dorm);
        count++;
        current = current->next;
    }

    fclose(file);
    printf("成功导出 %d 条记录到CSV文件 %s\n", count, filename);
    return 1;
}

// 导入CSV文件
Student* importFromCSV(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件 %s！\n", filename);
        return NULL;
    }

    Student* head = NULL;
    Student* tail = NULL;
    char line[256];
    int count = 0;

    // 跳过标题行
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        // 移除换行符
        line[strcspn(line, "\n")] = '\0';
        line[strcspn(line, "\r")] = '\0'; // 处理Windows换行

        // 处理CSV格式，移除引号
        char cleanedLine[256];
        int j = 0;
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] != '"') {
                cleanedLine[j++] = line[i];
            }
        }
        cleanedLine[j] = '\0';

        // 分割字符串
        char* token = strtok(cleanedLine, ",");
        if (token == NULL) continue;

        // 创建新节点
        Student* newStudent = (Student*)malloc(sizeof(Student));
        if (newStudent == NULL) {
            printf("内存分配失败！\n");
            fclose(file);
            return head;
        }

        // 学号
        strcpy(newStudent->id, token);

        // 姓名
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strcpy(newStudent->name, token);

        // 性别
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strcpy(newStudent->gender, token);

        // 出生年月
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strcpy(newStudent->birthdate, token);

        // 专业
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strcpy(newStudent->major, token);

        // 班级
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strcpy(newStudent->class_name, token);

        // 家庭地址
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strcpy(newStudent->address, token);

        // 宿舍号码
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strcpy(newStudent->dorm, token);

        newStudent->next = NULL;
        count++;

        // 添加到链表
        if (head == NULL) {
            head = newStudent;
            tail = newStudent;
        }
        else {
            tail->next = newStudent;
            tail = newStudent;
        }
    }

    fclose(file);
    printf("从CSV文件 %s 导入了 %d 条记录\n", filename, count);
    return head;
}


// 加密函数（简单的异或加密）
void encryptPassword(char* password) {
    char key = 'S'; // 加密密钥
    int len = strlen(password);
    for (int i = 0; i < len; i++) {
        password[i] = password[i] ^ key;
    }
}

// 设置密码
void setPassword() {
    char password[50];
    char confirm[50];

    printf("======== 设置密码 ========\n");

    while (1) {
        safeInput(password, sizeof(password), "请输入新密码(6-20位): ");

        if (strlen(password) < 6) {
            printf("密码长度不能少于6位！\n");
            continue;
        }

        if (strlen(password) > 20) {
            printf("密码长度不能超过20位！\n");
            continue;
        }

        safeInput(confirm, sizeof(confirm), "请再次输入密码: ");

        if (strcmp(password, confirm) == 0) {
            break;
        }
        else {
            printf("两次输入的密码不一致，请重新设置！\n");
        }
    }

    // 加密密码
    encryptPassword(password);

    // 保存到文件
    FILE* file = fopen(PASSWORD_FILE, "w");
    if (file != NULL) {
        fprintf(file, "%s", password);
        fclose(file);
        printf("密码设置成功！\n");
    }
    else {
        printf("无法保存密码到文件！\n");
    }

    pauseScreen();
}

// 检查密码
int checkPassword() {
    // 读取密码文件
    FILE* file = fopen(PASSWORD_FILE, "r");
    char storedPassword[50] = "";

    if (file == NULL) {
        // 如果文件不存在，使用默认密码
        strcpy(storedPassword, DEFAULT_PASSWORD);
        encryptPassword(storedPassword);
    }
    else {
        fgets(storedPassword, sizeof(storedPassword), file);
        storedPassword[strcspn(storedPassword, "\n")] = '\0';
        fclose(file);
    }

    // 用户输入密码
    char inputPassword[50];
    int attempts = 3;

    while (attempts > 0) {
        printf("请输入密码(剩余尝试次数: %d): ", attempts);

        // 隐藏密码输入（简单实现）
        int i = 0;
        char ch;
        while (1) {
            ch = getchar();
            if (ch == '\n' || ch == '\r') {
                inputPassword[i] = '\0';
                break;
            }
            else if (ch == 8 || ch == 127) { // 退格键
                if (i > 0) {
                    i--;
                    printf("\b \b");
                }
            }
            else {
                inputPassword[i++] = ch;
                printf("*");
            }
        }

        // 加密输入的密码进行对比
        char encryptedInput[50];
        strcpy(encryptedInput, inputPassword);
        encryptPassword(encryptedInput);

        if (strcmp(encryptedInput, storedPassword) == 0) {
            printf("\n密码正确！\n");
            return 1;
        }
        else {
            attempts--;
            printf("\n密码错误！\n");
        }
    }

    printf("密码错误次数过多，操作被拒绝！\n");
    return 0;
}

// 修改密码
void changePassword() {
    clearScreen();

    printf("======== 修改密码 ========\n");

    // 验证旧密码
    if (!checkPassword()) {
        printf("旧密码验证失败！\n");
        pauseScreen();
        return;
    }

    // 设置新密码
    setPassword();
}

// 密码验证（简化版，用于修改功能）
int verifyPassword() {
    return checkPassword();
}

