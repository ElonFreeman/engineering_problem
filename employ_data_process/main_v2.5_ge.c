#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

// 定义数据结构来存储行业名称和人数
typedef struct {
    char industry[100]; 
    int employment;     
} EmploymentData;

#define MAX_RECORDS 450 

// ----------------------------------------------------------------
// 工具函数：极致清理字符串开头和结尾的空白/控制字符
// ----------------------------------------------------------------
void trim_whitespace(char *str) {
    char *start = str;
    char *end = str + strlen(str) - 1;

    // 1. 从开头清除空白和控制字符
    while (*start != '\0' && (isspace((unsigned char)*start) || !isprint((unsigned char)*start))) {
        start++;
    }

    // 2. 从末尾清除空白和控制字符 (特别处理 \r 和 \n)
    while (end >= start && (*end == '\n' || *end == '\r' || isspace((unsigned char)*end) || !isprint((unsigned char)*end))) {
        end--;
    }

    // 3. 截断字符串
    *(end + 1) = '\0';

    // 4. 将处理后的字符串移回原位置
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

// ----------------------------------------------------------------
// 2. 从文件读取数据 (采用更健壮的 strtok 逻辑)
// ----------------------------------------------------------------
int read_data(const char *filename, EmploymentData arr[]) {
    // 使用 "rb" 模式打开文件，避免 Windows 自动转换 \r\n
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
                // 1. 清理行业名称
                trim_whitespace(industry_buffer);
                
                // 2. 清理人数（移除末尾的 \n, \r 等）
                trim_whitespace(token);

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


// --- 其他函数 (排序、写入、查询) 保持不变 ---

// 1. 选择排序（降序）
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

// 3. 写入文件
void write_sorted_data(const char *filename, const EmploymentData arr[], int n) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("错误：无法写入文件 %s\n", filename);
        return;
    }
    // 注意：这里写入时使用 \t 分隔，Windows环境下会使用 \r\n 换行
    fprintf(file, "行业名称\t就业人数\n"); 
    for (int i = 0; i < n; i++) {
        fprintf(file, "%s\t%d\n", arr[i].industry, arr[i].employment);
    }
    fclose(file);
}

// 4. 线性查询
int linear_search(const EmploymentData arr[], int n, const char *industry_name) {
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].industry, industry_name) == 0) {
            return arr[i].employment; 
        }
    }
    return -1; 
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
    printf("成功读取 %d 条记录（总数：396条）。\n", num_records); // num_records 现在应该显示 396

    // 2. 排序
    selection_sort_desc(records, num_records);
    printf("数据已成功进行降序排序。\n");

    // 3. 显示结果 (只显示前10条)
    printf("\n--- 排序后的数据结果（按人数降序）---\n");
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
        
        // 查询前清理用户输入
        trim_whitespace(query_industry);

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