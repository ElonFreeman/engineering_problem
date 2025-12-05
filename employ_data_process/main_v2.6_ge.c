#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

// 定义数据结构来存储行业名称和人数
typedef struct {
    // 行业名称，存储清理后的标准化字符串
    char industry[100]; 
    int employment;     
} EmploymentData;

#define MAX_RECORDS 450 

// ----------------------------------------------------------------
// 工具函数：核心修正！移除字符串中所有空白字符，并进行标准化
// ----------------------------------------------------------------
void standardize_string(char *str) {
    char *read_ptr = str;
    char *write_ptr = str;

    // 遍历整个字符串
    while (*read_ptr != '\0') {
        // 使用 isspace 检查是否为标准空白字符（空格、\t, \n, \r 等）
        if (!isspace((unsigned char)*read_ptr)) {
            // 如果不是空白字符，则保留，并将其复制到 write_ptr 指向的位置
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }
    // 在新字符串的末尾添加终止符
    *write_ptr = '\0';
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
// 2. 从文件读取数据 (将行业名称标准化后存入数组)
// ----------------------------------------------------------------
int read_data(const char *filename, EmploymentData arr[]) {
    FILE *file = fopen("E:/c_projects/C_home_works/employ_data_process/employ-data.csv", "r"); 
    if (file == NULL) {
        printf("错误：无法打开文件 %s\n", filename);
        return 0; 
    }

    int count = 0;
    char line[256]; 
    char *token;

    while (count < MAX_RECORDS && fgets(line, sizeof(line), file) != NULL) {
        // 使用 strtok 按制表符 '\t' 分割
        token = strtok(line, "\t"); 

        if (token != NULL) {
            // 第一个 token 是行业名称
            char industry_buffer[100];
            strcpy(industry_buffer, token);
            
            // 获取第二个 token：就业人数
            token = strtok(NULL, "\t"); 

            if (token != NULL) {
                // **关键：** 对行业名称进行彻底标准化
                standardize_string(industry_buffer);
                
                // 仅对人数进行标准化，以移除末尾的 \n, \r 等
                standardize_string(token);
                
                // 仅当行业名称非空时才存储
                if (strlen(industry_buffer) > 0) {
                    strncpy(arr[count].industry, industry_buffer, sizeof(arr[count].industry) - 1);
                    arr[count].industry[sizeof(arr[count].industry) - 1] = '\0';
                    
                    arr[count].employment = atoi(token);
                    
                    count++;
                }
            }
        }
    }

    fclose(file);
    return count;
}


// ----------------------------------------------------------------
// 3. 写入文件
// ----------------------------------------------------------------
void write_sorted_data(const char *filename, const EmploymentData arr[], int n) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("错误：无法写入文件 %s\n", filename);
        return;
    }
    // 注意：写入文件时，使用标准化后的无空格名称
    fprintf(file, "行业名称\t就业人数\n"); 
    for (int i = 0; i < n; i++) {
        fprintf(file, "%s\t%d\n", arr[i].industry, arr[i].employment);
    }
    fclose(file);
}

// ----------------------------------------------------------------
// 4. 线性查询
// ----------------------------------------------------------------
int linear_search(const EmploymentData arr[], int n, const char *industry_name) {
    for (int i = 0; i < n; i++) {
        // 直接比较两个标准化后的字符串
        if (strcmp(arr[i].industry, industry_name) == 0) {
            return arr[i].employment; 
        }
    }
    return -1; 
}

// ----------------------------------------------------------------
// 5. 主函数 (对用户输入进行标准化)
// ----------------------------------------------------------------
int main() {
    EmploymentData records[MAX_RECORDS];
    const char *input_file = "employ-data.csv";
    const char *output_file = "employ-sort.txt";
    int num_records;

    printf("--- 就业数据处理程序 ---\n");

    // 1. 读取数据 (使用标准化存储)
    num_records = read_data(input_file, records);
    if (num_records == 0) {
        printf("读取数据失败，请检查文件格式和编码，程序退出。\n");
        return 1;
    }
    printf("成功读取 %d 条记录（总数：396条）。\n", num_records); 

    // 2. 排序
    selection_sort_desc(records, num_records);
    printf("数据已成功进行降序排序。\n");

    // 3. 显示结果 (只显示前10条)
    printf("\n--- 排序后的数据结果（按人数降序）---\n");
    printf("注意：显示的行业名称已进行内部空格清理，以确保查询准确。\n");
    printf("%-40s %s\n", "行业名称", "就业人数");
    for (int i = 0; i < (num_records > 10 ? 10 : num_records); i++) {
        printf("%-40s %d\n", records[i].industry, records[i].employment);
    }
    if (num_records > 10) {
        printf("[...已省略 %d 条记录]\n", num_records - 10);
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
    
    // 清理输入缓冲区
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 

    while (1) {
        printf("请输入要查询的行业名称: ");
        if (fgets(query_industry, sizeof(query_industry), stdin) == NULL) {
            break; 
        }
        
        // **关键修正：对用户输入进行彻底标准化**
        standardize_string(query_industry);

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