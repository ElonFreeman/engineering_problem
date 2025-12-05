#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // 用于 isspace 函数

// 定义数据结构来存储行业名称和人数
typedef struct {
    char industry[100]; 
    int employment;     
} EmploymentData;

#define MAX_RECORDS 450 

// ----------------------------------------------------------------
// 工具函数：移除字符串开头和结尾的空白字符
// ----------------------------------------------------------------
void trim_whitespace(char *str) {
    char *end;

    // 1. 移除字符串开头的空白字符
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0) // 字符串为空，直接返回
        return;

    // 2. 移除字符串末尾的空白字符
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // 3. 添加新的字符串终止符
    *(end + 1) = 0;

    // 4. 将处理后的字符串移回原位置 (如果开头有空白被移除)
    if(str != (end + 1 - strlen(str))){
        memmove(end + 1 - strlen(str), str, strlen(str) + 1);
    }
}


// ----------------------------------------------------------------
// 1. 选择排序（降序）- 保持不变
// ----------------------------------------------------------------
void selection_sort_desc(EmploymentData arr[], int n) {
    int i, j, max_idx;
    for (i = 0; i < n - 1; i++) {
        max_idx = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j].employment > arr[max_idx].employment) {
                max_idx = j;
            }
        }
        if (max_idx != i) {
            EmploymentData temp = arr[i];
            arr[i] = arr[max_idx];
            arr[max_idx] = temp;
        }
    }
}

// ----------------------------------------------------------------
// 2. 从文件读取数据 (确保读取后即时清理数据)
// ----------------------------------------------------------------
int read_data(const char *filename, EmploymentData arr[]) {
    FILE *file = fopen("E:/c_projects/C_home_works/employ_data_process/employ-data.csv", "r");
    if (file == NULL) {
        printf("错误：无法打开文件 %s\n", filename);
        return 0; 
    }

    int count = 0;
    char line[256]; 

    while (count < MAX_RECORDS && fgets(line, sizeof(line), file) != NULL) {
        char *industry_str = line;
        char *employment_str = strchr(line, '\t'); 

        if (employment_str != NULL) {
            *employment_str = '\0';
            employment_str++; 

            // 1. 清理行业名称字符串
            trim_whitespace(industry_str);
            
            // 2. 清理人数字符串（仅移除末尾的换行符）
            size_t len = strlen(employment_str);
            if (len > 0 && employment_str[len - 1] == '\n') {
                employment_str[len - 1] = '\0';
            }

            // 复制数据
            strncpy(arr[count].industry, industry_str, sizeof(arr[count].industry) - 1);
            arr[count].industry[sizeof(arr[count].industry) - 1] = '\0';
            
            arr[count].employment = atoi(employment_str);
            
            count++;
        }
    }

    fclose(file);
    return count;
}

// ----------------------------------------------------------------
// 3. 将排序后的数据写入文件 - 保持不变
// ----------------------------------------------------------------
void write_sorted_data(const char *filename, const EmploymentData arr[], int n) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("错误：无法写入文件 %s\n", filename);
        return;
    }
    fprintf(file, "行业名称\t就业人数\n");
    for (int i = 0; i < n; i++) {
        fprintf(file, "%s\t%d\n", arr[i].industry, arr[i].employment);
    }
    fclose(file);
}

// ----------------------------------------------------------------
// 4. 线性查询 - 保持不变
// ----------------------------------------------------------------
int linear_search(const EmploymentData arr[], int n, const char *industry_name) {
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].industry, industry_name) == 0) {
            return arr[i].employment; 
        }
    }
    return -1; 
}

// ----------------------------------------------------------------
// 5. 主函数 (查询前清理用户输入)
// ----------------------------------------------------------------
int main() {
    EmploymentData records[MAX_RECORDS];
    const char *input_file = "employ-data.csv";
    const char *output_file = "employ-sort.txt";
    int num_records;

    printf("--- 就业数据处理程序 ---\n");

    // 1. 读取数据
    num_records = read_data(input_file, records);
    if (num_records == 0) {
        printf("读取数据失败，程序退出。\n");
        return 1;
    }
    printf("成功读取 %d 条记录（总数：396条）。\n", num_records);

    // 2. 排序
    selection_sort_desc(records, num_records);
    printf("数据已成功进行降序排序。\n");

    // 3. 显示结果
    printf("\n--- 排序后的数据结果（按人数降序）---\n");
    printf("%-40s %s\n", "行业名称", "就业人数");
    for (int i = 0; i < num_records; i++) {
        printf("%-40s %d\n", records[i].industry, records[i].employment);
    }
    printf("----------------------------------------------------\n");

    // 4. 写入文件
    write_sorted_data(output_file, records, num_records);
    printf("排序结果已写入文件：%s\n", output_file);

    // 5. 循环查询功能
    char query_industry[100];
    int employment_count;
    
    printf("\n--- 行业就业人数查询 ---\n");
    printf("输入 '退出' 或 'exit' 结束查询。\n");
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // 清理输入缓冲区

    while (1) {
        printf("请输入要查询的行业名称: ");
        if (fgets(query_industry, sizeof(query_industry), stdin) == NULL) {
            break; 
        }
        
        // **关键修正：在查询前清理用户输入的字符串**
        trim_whitespace(query_industry);

        // 检查退出命令
        if (strcmp(query_industry, "退出") == 0 || strcmp(query_industry, "exit") == 0) {
            break;
        }

        // 查询
        employment_count = linear_search(records, num_records, query_industry);

        if (employment_count != -1) {
            printf("查询成功！行业 [%s] 的就业人数为: %d\n", query_industry, employment_count);
        } else {
            printf("查询不成功！未找到行业 [%s] 的记录。\n", query_industry);
        }
    }

    printf("\n查询程序已结束。再见！\n");
    
    return 0;
}