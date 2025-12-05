#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义数据结构来存储行业名称和人数
typedef struct {
    char industry[100]; // 行业名称，假设最长不超过99个字符
    int employment;     // 就业人数
} EmploymentData;

// **重要修正：将最大记录数增加到足够大，以容纳所有 396 条数据**
#define MAX_RECORDS 450 

// ----------------------------------------------------------------
// 1. 选择排序（降序）
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
// 2. 从文件读取数据 (改进的读取逻辑)
// ----------------------------------------------------------------
int read_data(const char *filename, EmploymentData arr[]) {
    FILE *file = fopen("E:/c_projects/C_home_works/employ_data_process/employ-data.csv", "r");
    if (file == NULL) {
        printf("错误：无法打开文件 %s\n", filename);
        return 0; 
    }

    int count = 0;
    char line[256]; // 缓冲区，假设每行不超过 255 个字符

    while (count < MAX_RECORDS && fgets(line, sizeof(line), file) != NULL) {
        // 使用 strtok 分割行：先找制表符 '\t'
        char *industry_str = line;
        char *employment_str = strchr(line, '\t'); 

        if (employment_str != NULL) {
            // 将制表符替换为字符串终止符 '\0'，从而分隔出行业名称
            *employment_str = '\0';
            employment_str++; // 指向人数字符串的起始位置

            // 移除人数字符串末尾的换行符 '\n'
            size_t len = strlen(employment_str);
            if (len > 0 && employment_str[len - 1] == '\n') {
                employment_str[len - 1] = '\0';
            }

            // 复制数据到结构体
            // 确保行业名称不会溢出
            strncpy(arr[count].industry, industry_str, sizeof(arr[count].industry) - 1);
            arr[count].industry[sizeof(arr[count].industry) - 1] = '\0';
            
            // 将人数字符串转换为整数
            arr[count].employment = atoi(employment_str);
            
            count++;
        }
    }

    fclose(file);
    return count;
}

// ----------------------------------------------------------------
// 3. 将排序后的数据写入文件
// ----------------------------------------------------------------
void write_sorted_data(const char *filename, const EmploymentData arr[], int n) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("错误：无法写入文件 %s\n", filename);
        return;
    }

    // 写入表头
    fprintf(file, "行业名称\t就业人数\n");

    // 写入排序后的数据
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
        // 使用 strcmp 比较字符串，如果相同则返回 0
        if (strcmp(arr[i].industry, industry_name) == 0) {
            return arr[i].employment; // 查询成功，返回人数
        }
    }
    return -1; // 查询不成功，返回 -1
}

// ----------------------------------------------------------------
// 5. 主函数
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

    // 2. 排序（选择排序，降序）
    selection_sort_desc(records, num_records);
    printf("数据已成功进行降序排序。\n");

    // 3. 显示排序结果
    printf("\n--- 排序后的数据结果（按人数降序）---\n");
    printf("%-40s %s\n", "行业名称", "就业人数");
    for (int i = 0; i < num_records; i++) {
        printf("%-40s %d\n", records[i].industry, records[i].employment);
    }
    printf("----------------------------------------------------\n");

    // 4. 写入排序结果到文件
    write_sorted_data(output_file, records, num_records);
    printf("排序结果已写入文件：%s\n", output_file);

    // 5. 循环查询功能
    char query_industry[100];
    int employment_count;
    
    printf("\n--- 行业就业人数查询 ---\n");
    printf("输入 '退出' 或 'exit' 结束查询。\n");
    
    // 清理输入缓冲区（避免之前可能的残留换行符影响 fgets）
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    while (1) {
        printf("请输入要查询的行业名称: ");
        // 使用 fgets 读取整行输入
        if (fgets(query_industry, sizeof(query_industry), stdin) == NULL) {
            break; 
        }
        
        // 移除末尾的换行符
        size_t len = strlen(query_industry);
        if (len > 0 && query_industry[len - 1] == '\n') {
            query_industry[len - 1] = '\0';
        }

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