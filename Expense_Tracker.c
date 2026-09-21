#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

struct Expense {
    int id;
    char name[50];
    char category[30];
    float amount;
};

struct Expense expenses[MAX];
int count = 0;

// File mein data save karne ke liye
void saveToFile() {
    FILE *fp = fopen("expenses.txt", "w");
    if (!fp) return;
    fprintf(fp, "%d\n", count);
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d|%s|%s|%.2f\n", expenses[i].id, expenses[i].name, expenses[i].category, expenses[i].amount);
    }
    fclose(fp);
}

// File se data load karne ke liye
void loadFromFile() {
    FILE *fp = fopen("expenses.txt", "r");
    if (!fp) return;
    if (fscanf(fp, "%d\n", &count) != 1) { count = 0; fclose(fp); return; }
    for (int i = 0; i < count; i++) {
        fscanf(fp, "%d|%49[^|]|%29[^|]|%f\n", &expenses[i].id, expenses[i].name, expenses[i].category, &expenses[i].amount);
    }
    fclose(fp);
}

// 1. Add Expense
void addExpense() {
    if (count >= MAX) { printf("List is full!\n"); return; }
    expenses[count].id = count + 1;
    
    printf("Enter expense name: ");
    scanf("%s", expenses[count].name);
    printf("Enter category: ");
    scanf("%s", expenses[count].category);
    printf("Enter amount: ");
    scanf("%f", &expenses[count].amount);
    
    count++;
    saveToFile();
    printf("Expense added successfully!\n");
}

// 2. View Expenses
void viewExpenses() {
    if (count == 0) { printf("No expenses recorded yet.\n"); return; }
    
    printf("\n--- EXPENSE LIST ---\n");
    printf("ID\tName\t\tCategory\tAmount\n");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%s\t\t%s\t\t%.2f\n", expenses[i].id, expenses[i].name, expenses[i].category, expenses[i].amount);
    }
}

// 3. Search Expense
void searchExpense() {
    char key[50];
    int found = 0;
    printf("Enter name or category to search: ");
    scanf("%s", key);
    
    for (int i = 0; i < count; i++) {
        if (strstr(expenses[i].name, key) != NULL || strstr(expenses[i].category, key) != NULL) {
            printf("[%d] %s | %s | %.2f\n", expenses[i].id, expenses[i].name, expenses[i].category, expenses[i].amount);
            found = 1;
        }
    }
    if (!found) printf("No matching expense found.\n");
}

// 4. Edit Expense
void editExpense() {
    int id;
    printf("Enter expense ID to edit: ");
    scanf("%d", &id);
    int idx = id - 1;
    
    if (idx < 0 || idx >= count) { printf("Expense not found.\n"); return; }
    
    printf("Enter new name: ");
    scanf("%s", expenses[idx].name);
    printf("Enter new category: ");
    scanf("%s", expenses[idx].category);
    printf("Enter new amount: ");
    scanf("%f", &expenses[idx].amount);
    
    saveToFile();
    printf("Expense updated successfully!\n");
}

// 5. Delete Expense
void deleteExpense() {
    int id;
    printf("Enter expense ID to delete: ");
    scanf("%d", &id);
    int idx = id - 1;
    
    if (idx < 0 || idx >= count) { printf("Expense not found.\n"); return; }
    
    for (int i = idx; i < count - 1; i++) {
        expenses[i] = expenses[i + 1];
        expenses[i].id = i + 1; // ID re-adjust karna
    }
    count--;
    saveToFile();
    printf("Expense deleted successfully!\n");
}

// 6. Show Summary
void showSummary() {
    if (count == 0) { printf("No expenses recorded yet.\n"); return; }
    
    float total = 0;
    int high = 0, low = 0;
    
    for (int i = 0; i < count; i++) {
        total += expenses[i].amount;
        if (expenses[i].amount > expenses[high].amount) high = i;
        if (expenses[i].amount < expenses[low].amount) low = i;
    }
    
    printf("\nTotal expenses: %.2f\n", total);
    printf("Highest expense: %s (%.2f)\n", expenses[high].name, expenses[high].amount);
    printf("Lowest expense: %s (%.2f)\n", expenses[low].name, expenses[low].amount);
}

int main() {
    int choice;
    loadFromFile(); // Program start hote hi data load hoga

    while (1) {
        printf("\n=== EXPENSE TRACKER ===\n");
        printf("1. Add Expense\n");
        printf("2. View Expenses\n");
        printf("3. Search Expense\n");
        printf("4. Edit Expense\n");
        printf("5. Delete Expense\n");
        printf("6. Show Summary\n");
        printf("7. Save & Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addExpense(); break;
            case 2: viewExpenses(); break;
            case 3: searchExpense(); break;
            case 4: editExpense(); break;
            case 5: deleteExpense(); break;
            case 6: showSummary(); break;
            case 7: 
                saveToFile();
                printf("\nData saved. Goodbye!\n");
                return 0;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
    return 0;
}
