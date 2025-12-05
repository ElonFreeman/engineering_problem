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

int main() {
    FILE *dataset = fopen("E:/c_projects/C_home_works/employ_data_process/employ-data.csv", "r");
    if (dataset == NULL) {
        printf("Cannot open the dataset file.");
        return 1;
    }

    EmploymentData main_data_set[MAX_ROWS];
    char line[MAX_LINE];
    int count = 0;
    
    // Read data file
    while (fgets(line, sizeof(line), dataset) != NULL && count < MAX_ROWS) {
        // Find tab or space separator
        char *tab_pos = strchr(line, '\t');
        if (tab_pos == NULL) {
            // If no tab found, try space separator
            tab_pos = strchr(line, ' ');
        }
        
        if (tab_pos != NULL) {
            // Extract industry name (part before tab/space)
            int industry_len = tab_pos - line;
            if (industry_len >= MAX_TEXT_LEN) {
                industry_len = MAX_TEXT_LEN - 1;
            }
            strncpy(main_data_set[count].industry, line, industry_len);
            main_data_set[count].industry[industry_len] = '\0';
            
            // Extract employment number (part after tab/space)
            char *num_start = tab_pos + 1;
            // Skip possible spaces
            while (*num_start == ' ') {
                num_start++;
            }
            main_data_set[count].employees = atoi(num_start);
            
            count++;
        }
    }
    fclose(dataset);
    
    // Use selection sort algorithm to sort data in descending order
    selectionSort(main_data_set, count);
    
    // Display sorted data
    printf("Sorted employment data (descending order by employee count):\n");
    printf("=============================================================\n");
    for (int i = 0; i < count; i++) {
        printf("%-40s %d\n", main_data_set[i].industry, main_data_set[i].employees);
    }
    
    // Write sorted results to file
    FILE *output = fopen("employ-sort.txt", "w");
    if (output != NULL) {
        for (int i = 0; i < count; i++) {
            fprintf(output, "%-40s %d\n", main_data_set[i].industry, main_data_set[i].employees);
        }
        fclose(output);
        printf("\nSorted results have been written to employ-sort.txt file\n");
    } else {
        printf("\nCannot create output file employ-sort.txt\n");
    }
    
    // Search functionality
    char search_industry[MAX_TEXT_LEN];
    char continue_search;
    
    printf("\n=== Industry Employment Query ===\n");
    
    do {
        printf("Please enter the industry name to query: ");
        fgets(search_industry, sizeof(search_industry), stdin);
        
        // Remove newline character
        size_t len = strlen(search_industry);
        if (len > 0 && search_industry[len - 1] == '\n') {
            search_industry[len - 1] = '\0';
        }
        
        // Execute search
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(main_data_set[i].industry, search_industry) == 0) {
                printf("Query successful! %s employment count: %d\n", 
                       main_data_set[i].industry, main_data_set[i].employees);
                found = 1;
                break;
            }
        }
        
        if (!found) {
            printf("Query failed! Industry not found: %s\n", search_industry);
        }
        
        printf("Continue query? (y/n): ");
        continue_search = getchar();
        getchar(); // Consume newline character
        
    } while (continue_search == 'y' || continue_search == 'Y');
    
    printf("Program ended. Thank you for using!\n");
    
    return 0;
}