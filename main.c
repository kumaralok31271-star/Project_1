#include <stdio.h>
#include <stdlib.h>

#define MAX 100

// Structure to store expense details
struct Expense {
    int id;
    char name[50];
    float amount;
};

struct Expense expenses[MAX];
int count = 0;

// Function to add a new expense
void addExpense() {
    if (count >= MAX) {
        printf("\nList is full!\n");
        return;
    }
    
    expenses[count].id = count + 1;
    
    printf("\nEnter expense name: ");
    scanf("%s", expenses[count].name);
    
    printf("Enter amount: ");
    scanf("%f", &expenses[count].amount);
    
    count++;
    printf("\nExpense added successfully!\n");
}

// Function to view all expenses
void viewExpenses() {
    if (count == 0) {
        printf("\nNo expenses recorded yet.\n");
        return;
    }
    
    printf("\n--- EXPENSE LIST ---\n");
    printf("ID\tName\t\tAmount\n");
    printf("----------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%s\t\t%.2f\n", expenses[i].id, expenses[i].name, expenses[i].amount);
    }
}

// Function to save data to a file
void saveToFile() {
    FILE *fp = fopen("expenses.txt", "w");
    if (fp == NULL) {
        printf("\nError opening file!\n");
        return;
    }
    
    fprintf(fp, "%d\n", count);
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d %s %.2f\n", expenses[i].id, expenses[i].name, expenses[i].amount);
    }
    
    fclose(fp);
    printf("\nData saved to file successfully!\n");
}

// Function to load data from a file
void loadFromFile() {
    FILE *fp = fopen("expenses.txt", "r");
    if (fp == NULL) return; // File doesn't exist yet
    
    fscanf(fp, "%d", &count);
    for (int i = 0; i < count; i++) {
        fscanf(fp, "%d %s %f", &expenses[i].id, expenses[i].name, &expenses[i].amount);
    }
    
    fclose(fp);
}

int main() {
    int choice;
    loadFromFile(); // Load existing data when program starts

    while (1) {
        printf("\n=== SIMPLE EXPENSE TRACKER ===\n");
        printf("1. Add Expense\n");
        printf("2. View Expenses\n");
        printf("3. Save & Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addExpense();
                break;
            case 2:
                viewExpenses();
                break;
            case 3:
                saveToFile();
                printf("\nExiting program. Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }

    return 0;
}
