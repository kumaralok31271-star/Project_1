#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

struct Expense
{
    int id;
    char date[20];
    float amount;
    char category[30];
    char description[100];
    char paymentMode[20];
};

struct Expense expenses[MAX];

int count = 0;
float budget = 0.0;

/* ---------- Helper Function ---------- */

void removeNewline(char str[])
{
    str[strcspn(str, "\n")] = '\0';
}

void readString(char str[], int size)
{
    fgets(str, size, stdin);
    removeNewline(str);
}

float readAmount()
{
    float amount;

    while (1)
    {
        scanf("%f", &amount);
        getchar();

        if (amount > 0)
            return amount;

        printf("Invalid amount! Enter a positive value: ");
    }
}

/* ---------- File Handling ---------- */

void saveToFile()
{
    FILE *file = fopen("expenses.txt", "w");

    if (file == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "%d\n", count);

    for (int i = 0; i < count; i++)
    {
        fprintf(file, "%d|%s|%.2f|%s|%s|%s\n",
                expenses[i].id,
                expenses[i].date,
                expenses[i].amount,
                expenses[i].category,
                expenses[i].description,
                expenses[i].paymentMode);
    }

    fclose(file);

    /* Save budget separately */
    FILE *budgetFile = fopen("budget.txt", "w");

    if (budgetFile != NULL)
    {
        fprintf(budgetFile, "%.2f", budget);
        fclose(budgetFile);
    }
}

void loadFromFile()
{
    FILE *file = fopen("expenses.txt", "r");

    if (file == NULL)
        return;

    fscanf(file, "%d\n", &count);

    if (count > MAX)
        count = MAX;

    for (int i = 0; i < count; i++)
    {
        fscanf(file,
               "%d|%19[^|]|%f|%29[^|]|%99[^|]|%19[^\n]\n",
               &expenses[i].id,
               expenses[i].date,
               &expenses[i].amount,
               expenses[i].category,
               expenses[i].description,
               expenses[i].paymentMode);
    }

    fclose(file);

    /* Load budget */
    FILE *budgetFile = fopen("budget.txt", "r");

    if (budgetFile != NULL)
    {
        fscanf(budgetFile, "%f", &budget);
        fclose(budgetFile);
    }
}

/* ---------- Add Expense ---------- */

void addExpense()
{
    if (count >= MAX)
    {
        printf("\nExpense storage is full!\n");
        return;
    }

    expenses[count].id = count + 1;

    printf("\nEnter Date (DD-MM-YYYY): ");
    readString(expenses[count].date,
               sizeof(expenses[count].date));

    printf("Enter Amount: ");
    expenses[count].amount = readAmount();

    printf("Enter Category: ");
    readString(expenses[count].category,
               sizeof(expenses[count].category));

    printf("Enter Description: ");
    readString(expenses[count].description,
               sizeof(expenses[count].description));

    printf("Enter Payment Mode (Cash/Card/UPI): ");
    readString(expenses[count].paymentMode,
               sizeof(expenses[count].paymentMode));

    count++;

    saveToFile();

    printf("\nExpense added successfully!\n");
}

/* ---------- Display Expenses ---------- */

void viewExpenses()
{
    if (count == 0)
    {
        printf("\nNo expense records found.\n");
        return;
    }

    printf("\n================ EXPENSE RECORDS ================\n");

    printf("%-4s %-12s %-10s %-15s %-25s %-12s\n",
           "ID",
           "Date",
           "Amount",
           "Category",
           "Description",
           "Payment");

    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%-4d %-12s %-10.2f %-15s %-25s %-12s\n",
               expenses[i].id,
               expenses[i].date,
               expenses[i].amount,
               expenses[i].category,
               expenses[i].description,
               expenses[i].paymentMode);
    }
}

/* ---------- Edit Expense ---------- */

void editExpense()
{
    int id;

    if (count == 0)
    {
        printf("\nNo expense records available.\n");
        return;
    }

    printf("\nEnter Expense ID to edit: ");
    scanf("%d", &id);
    getchar();

    if (id < 1 || id > count)
    {
        printf("Invalid Expense ID!\n");
        return;
    }

    int index = id - 1;

    printf("\nEnter new Date: ");
    readString(expenses[index].date,
               sizeof(expenses[index].date));

    printf("Enter new Amount: ");
    expenses[index].amount = readAmount();

    printf("Enter new Category: ");
    readString(expenses[index].category,
               sizeof(expenses[index].category));

    printf("Enter new Description: ");
    readString(expenses[index].description,
               sizeof(expenses[index].description));

    printf("Enter new Payment Mode: ");
    readString(expenses[index].paymentMode,
               sizeof(expenses[index].paymentMode));

    saveToFile();

    printf("\nExpense updated successfully!\n");
}

/* ---------- Delete Expense ---------- */

void deleteExpense()
{
    int id;

    if (count == 0)
    {
        printf("\nNo expense records available.\n");
        return;
    }

    printf("\nEnter Expense ID to delete: ");
    scanf("%d", &id);
    getchar();

    if (id < 1 || id > count)
    {
        printf("Invalid Expense ID!\n");
        return;
    }

    int index = id - 1;

    for (int i = index; i < count - 1; i++)
    {
        expenses[i] = expenses[i + 1];
    }

    count--;

    /* Reassign IDs */
    for (int i = 0; i < count; i++)
    {
        expenses[i].id = i + 1;
    }

    saveToFile();

    printf("\nExpense deleted successfully!\n");
}

/* ---------- Total Expenditure ---------- */

float calculateTotal()
{
    float total = 0;

    for (int i = 0; i < count; i++)
    {
        total += expenses[i].amount;
    }

    return total;
}

/* ---------- Category Summary ---------- */

void categorySummary()
{
    if (count == 0)
    {
        printf("\nNo expense records available.\n");
        return;
    }

    printf("\n============= CATEGORY-WISE SUMMARY =============\n");

    char categories[MAX][30];
    float totals[MAX];

    int categoryCount = 0;

    for (int i = 0; i < count; i++)
    {
        int found = -1;

        for (int j = 0; j < categoryCount; j++)
        {
            if (strcmp(categories[j], expenses[i].category) == 0)
            {
                found = j;
                break;
            }
        }

        if (found != -1)
        {
            totals[found] += expenses[i].amount;
        }
        else
        {
            strcpy(categories[categoryCount],
                   expenses[i].category);

            totals[categoryCount] =
                expenses[i].amount;

            categoryCount++;
        }
    }

    for (int i = 0; i < categoryCount; i++)
    {
        printf("%-20s : %.2f\n",
               categories[i],
               totals[i]);
    }
}

/* ---------- Summary ---------- */

void showSummary()
{
    if (count == 0)
    {
        printf("\nNo expense records available.\n");
        return;
    }

    float total = calculateTotal();

    float highest = expenses[0].amount;
    float lowest = expenses[0].amount;

    for (int i = 1; i < count; i++)
    {
        if (expenses[i].amount > highest)
            highest = expenses[i].amount;

        if (expenses[i].amount < lowest)
            lowest = expenses[i].amount;
    }

    printf("\n================ EXPENSE SUMMARY ================\n");

    printf("Total Expenses      : %.2f\n", total);
    printf("Highest Expense     : %.2f\n", highest);
    printf("Lowest Expense      : %.2f\n", lowest);
    printf("Number of Records   : %d\n", count);

    if (budget > 0)
    {
        printf("Set Budget          : %.2f\n", budget);
        printf("Remaining Budget    : %.2f\n",
               budget - total);

        if (total > budget)
        {
            printf("Budget Status       : Budget Exceeded\n");
        }
        else
        {
            printf("Budget Status       : Within Budget\n");
        }
    }
    else
    {
        printf("Budget              : Not Set\n");
    }
}

/* ---------- Budget Module ---------- */

void setBudget()
{
    printf("\nEnter your budget: ");
    budget = readAmount();

    saveToFile();

    printf("Budget set successfully!\n");
}

/* ---------- Main Menu ---------- */

int main()
{
    int choice;

    loadFromFile();

    while (1)
    {
        printf("\n\n==============================================\n");
        printf("             EXPENSE TRACKER\n");
        printf("==============================================\n");

        printf("1. Add Expense\n");
        printf("2. View Expenses\n");
        printf("3. Edit Expense\n");
        printf("4. Delete Expense\n");
        printf("5. Total & Summary\n");
        printf("6. Category-wise Summary\n");
        printf("7. Set Budget\n");
        printf("8. Exit\n");

        printf("==============================================\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
                addExpense();
                break;

            case 2:
                viewExpenses();
                break;

            case 3:
                editExpense();
                break;

            case 4:
                deleteExpense();
                break;

            case 5:
                showSummary();
                break;

            case 6:
                categorySummary();
                break;

            case 7:
                setBudget();
                break;

            case 8:
                saveToFile();
                printf("\nData saved successfully.\n");
                printf("Thank you for using Expense Tracker!\n");
                return 0;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }

    return 0;
}