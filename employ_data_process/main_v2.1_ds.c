#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1000
#define MAX_ROWS 1000
#define MAX_TEXT_LEN 100
#define MAX_NUM_LEN 20

typedef struct {
    char industry[MAX_TEXT_LEN];
    int employees;
} EmploymentData;

// Selection sort algorithm (descending order)
void selectionSort(EmploymentData data[], int count) {
    int i, j, max_idx;
    EmploymentData temp;
    
    for (i = 0; i < count - 1; i++) {
        max_idx = i;
        
        for (j = i + 1; j < count; j++) {
            if (data[j].employees > data[max_idx].employees) {
                max_idx = j;
            }
        }
        
        if (max_idx != i) {
            // Swap data
            temp = data[i];
            data[i] = data[max_idx];
            data[max_idx] = temp;
        }
    }
}

// Calculate display width for Chinese and English mixed string
int calculateDisplayWidth(const char *str) {
    int width = 0;
    while (*str) {
        // Chinese characters typically take 2 display positions
        if ((unsigned char)*str >= 0x80) {
            width += 2;
            str++; // Skip next byte for Chinese character
        } else {
            width += 1;
        }
        str++;
    }
    return width;
}

// Improved display function with better formatting
void displayData(EmploymentData data[], int count, int start, int end) {
    printf("Rank  %-40s %12s\n", "Industry", "Employees");
    printf("----- %-40s %12s\n", "----------------------------------------", "------------");
    
    for (int i = start; i < end && i < count; i++) {
        int industry_width = calculateDisplayWidth(data[i].industry);
        int padding = 40 - industry_width;
        if (padding < 0) padding = 0;
        
        printf("%-5d %s", i + 1, data[i].industry);
        
        // Add manual padding for alignment
        for (int p = 0; p < padding; p++) {
            printf(" ");
        }
        
        printf(" %12d\n", data[i].employees);
    }
}

int main() {
    FILE *dataset = fopen("E:/c_projects/C_home_works/employ_data_process/employ-data.csv", "r");
    if (dataset == NULL) {
        printf("Cannot open the dataset file 'employ-data.csv'\n");
        return 1;
    }

    EmploymentData main_data_set[MAX_ROWS];
    char line[MAX_LINE];
    int count = 0;
    
    printf("Reading data file...\n");
    
    // Read data file
    while (fgets(line, sizeof(line), dataset) != NULL && count < MAX_ROWS) {
        // Remove newline characters
        line[strcspn(line, "\n")] = 0;
        line[strcspn(line, "\r")] = 0;
        
        // Find the last space or tab
        char *last_space = strrchr(line, ' ');
        char *last_tab = strrchr(line, '\t');
        
        char *separator = (last_tab != NULL) ? last_tab : last_space;
        
        if (separator != NULL) {
            // Extract industry name
            int industry_len = separator - line;
            if (industry_len >= MAX_TEXT_LEN) {
                industry_len = MAX_TEXT_LEN - 1;
            }
            strncpy(main_data_set[count].industry, line, industry_len);
            main_data_set[count].industry[industry_len] = '\0';
            
            // Extract employment number
            char *num_start = separator + 1;
            while (*num_start == ' ') {
                num_start++;
            }
            main_data_set[count].employees = atoi(num_start);
            
            count++;
        }
    }
    fclose(dataset);
    
    printf("Successfully read %d records\n\n", count);
    
    // Sort data
    selectionSort(main_data_set, count);
    
    // Display top 30 records with better formatting
    printf("TOP 30 INDUSTRIES BY EMPLOYMENT:\n");
    printf("=================================\n");
    displayData(main_data_set, count, 0, 30);
    
    // Write full results to file with better formatting
    FILE *output = fopen("employ-sort.txt", "w");
    if (output != NULL) {
        fprintf(output, "Rank  %-40s %12s\n", "Industry", "Employees");
        fprintf(output, "----- %-40s %12s\n", "----------------------------------------", "------------");
        
        for (int i = 0; i < count; i++) {
            int industry_width = calculateDisplayWidth(main_data_set[i].industry);
            int padding = 40 - industry_width;
            if (padding < 0) padding = 0;
            
            fprintf(output, "%-5d %s", i + 1, main_data_set[i].industry);
            
            for (int p = 0; p < padding; p++) {
                fprintf(output, " ");
            }
            
            fprintf(output, " %12d\n", main_data_set[i].employees);
        }
        fclose(output);
        printf("\nFull sorted list has been written to employ-sort.txt\n");
    }
    
    // Search functionality
    char search_industry[MAX_TEXT_LEN];
    char continue_search;
    
    printf("\n=== INDUSTRY EMPLOYMENT QUERY ===\n");
    printf("Example: 国际组织, 房屋工程建筑, 综合零售\n\n");
    
    do {
        printf("Enter industry name: ");
        if (fgets(search_industry, sizeof(search_industry), stdin) == NULL) {
            break;
        }
        
        // Remove newline
        size_t len = strlen(search_industry);
        if (len > 0 && search_industry[len - 1] == '\n') {
            search_industry[len - 1] = '\0';
        }
        
        if (strlen(search_industry) == 0) {
            printf("Please enter an industry name.\n");
            continue;
        }
        
        // Execute search
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(main_data_set[i].industry, search_industry) == 0) {
                printf("\n✓ FOUND: %s\n", main_data_set[i].industry);
                printf("  Employees: %d\n", main_data_set[i].employees);
                printf("  Rank: #%d\n", i + 1);
                found = 1;
                break;
            }
        }
        
        if (!found) {
            printf("\n✗ Not found: %s\n", search_industry);
            printf("Check employ-sort.txt for complete list.\n");
        }
        
        printf("\nSearch again? (y/n): ");
        continue_search = getchar();
        getchar(); // Consume newline
        
    } while (continue_search == 'y' || continue_search == 'Y');
    
    printf("\nProgram completed.\n");
    
    return 0;
}