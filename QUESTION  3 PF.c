#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Employee
{
    int id;
    char name[50];
    char desig[50];
    float salary;
};

void read_line(char *buf, size_t size)
{
    if (fgets(buf, (int)size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
}

int read_int(const char *prompt)
{
    char buf[128];
    int value;
    while (1) {
        if (prompt) printf("%s", prompt);
        read_line(buf, sizeof(buf));
        if (sscanf(buf, "%d", &value) == 1) return value;
        printf("Invalid integer. Try again.\n");
    }
}

float read_float(const char *prompt)
{
    char buf[128];
    float value;
    while (1) {
        if (prompt) printf("%s", prompt);
        read_line(buf, sizeof(buf));
        if (sscanf(buf, "%f", &value) == 1) return value;
        printf("Invalid number. Try again.\n");
    }
}

/* case-insensitive compare for full strings */
int str_iequal(const char *a, const char *b)
{
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == '\0' && *b == '\0';
}

void display(struct Employee emp[], int n)
{
    printf("\n================== Employee Records ==================\n\n");
    for (int i = 0; i < n; i++)
    {
        printf("Employee %d:\n", i + 1);
        printf("ID: %d\n", emp[i].id);
        printf("Name: %s\n", emp[i].name);
        printf("Designation: %s\n", emp[i].desig);
        printf("Salary: %.2f\n", emp[i].salary);
        printf("=====================================================\n");
    }
}

void maxSal(struct Employee emp[], int n)
{
    if (n <= 0) return;
    int maxIndex = 0;
    for (int i = 1; i < n; i++)
    {
        if (emp[i].salary > emp[maxIndex].salary) maxIndex = i;
    }
    printf("\nEmployee With Highest Salary:\n");
    printf("ID: %d\n", emp[maxIndex].id);
    printf("Name: %s\n", emp[maxIndex].name);
    printf("Designation: %s\n", emp[maxIndex].desig);
    printf("Salary: %.2f\n", emp[maxIndex].salary);
}

void search(struct Employee emp[], int n)
{
    printf("\nSearch by:\n1. ID\n2. Name\n");
    int choice = read_int("Enter choice: ");

    if (choice == 1)
    {
        int id = read_int("Enter ID to search: ");
        for (int i = 0; i < n; i++)
        {
            if (emp[i].id == id)
            {
                printf("\nRecord Found:\n");
                printf("ID: %d\n", emp[i].id);
                printf("Name: %s\n", emp[i].name);
                printf("Designation: %s\n", emp[i].desig);
                printf("Salary: %.2f\n", emp[i].salary);
                return;
            }
        }
        printf("\nNo employee found with this ID.\n");
    }
    else if (choice == 2)
    {
        char name[50];
        printf("Enter Name to search: ");
        read_line(name, sizeof(name));

        for (int i = 0; i < n; i++)
        {
            if (str_iequal(emp[i].name, name))
            {
                printf("\nRecord Found:\n");
                printf("ID: %d\n", emp[i].id);
                printf("Name: %s\n", emp[i].name);
                printf("Designation: %s\n", emp[i].desig);
                printf("Salary: %.2f\n", emp[i].salary);
                return;
            }
        }
        printf("\nNo employee found with this name.\n");
    }
    else
    {
        printf("Invalid choice.\n");
    }
}

int main()
{
    int n = read_int("Enter number of employees: ");
    if (n <= 0) {
        printf("Number of employees must be positive.\n");
        return 1;
    }

    struct Employee *emp = malloc(sizeof(struct Employee) * n);
    if (!emp) {
        perror("malloc");
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        printf("\nEmployee %d:\n", i + 1);
        emp[i].id = read_int("Enter ID: ");

        printf("Enter Name: ");
        read_line(emp[i].name, sizeof(emp[i].name));

        printf("Enter Designation: ");
        read_line(emp[i].desig, sizeof(emp[i].desig));

        emp[i].salary = read_float("Enter Salary: ");
    }

    display(emp, n);
    maxSal(emp, n);
    search(emp, n);

    free(emp);
    return 0;
}
