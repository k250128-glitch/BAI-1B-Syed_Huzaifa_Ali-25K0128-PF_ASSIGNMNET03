#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int studentID;
    char fullName[100];
    char batch[20];         
    char membership[10];    
    char regDate[15];       
    char dob[15];           
    char interest[10];      
} Student;

Student *db = NULL;  // Dynamic array
int total = 0;      

void loadDatabase(const char *file) {
    FILE *fp = fopen(file, "rb");
    if (!fp) return;

    // Count records
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    total = size / sizeof(Student);
    db = malloc(total * sizeof(Student));

    if (!db) {
        printf("MEMORY ALLOCATE NHI HOSKTI KUCH GALTI KI HE , program is closing :(\n");
        exit(1);
    }

    fread(db, sizeof(Student), total, fp);
    fclose(fp);
}

void saveDatabase(const char *file) {
    FILE *fp = fopen(file, "wb");
    if (!fp) {
        printf("File NHI opening!\n");
        return;
    }
    fwrite(db, sizeof(Student), total, fp);
    fclose(fp);
}

void addStudent(Student s, const char *file) {
    for (int i = 0; i < total; i++) {
        if (db[i].studentID == s.studentID) {
            printf("ID already registered!\n");
            return;
        }
    }

    db = realloc(db, (total + 1) * sizeof(Student));
    db[total] = s;
    total++;

    saveDatabase(file);
    printf("Student added successfully!\n");
}

void updateStudent(int id, const char *file) {
    for (int i = 0; i < total; i++) {
        if (db[i].studentID == id) {
            printf("Enter new batch: ");
            scanf("%s", db[i].batch);

            printf("New Membership type (IEEE or ACM): ");
            scanf("%s", db[i].membership);

            saveDatabase(file);
            printf("Record updated!\n");
            return;
        }
    }
    printf("Record not found :\!\n");
}

void deleteStudent(int id, const char *file) {
    int index = -1;

    for (int i = 0; i < total; i++) {
        if (db[i].studentID == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Student not found!\n");
        return;
    }

    for (int i = index; i < total - 1; i++) {
        db[i] = db[i + 1];
    }

    total--;
    db = realloc(db, total * sizeof(Student));

    saveDatabase(file);
    printf("Record deleted!\n");
}

void showAll() {
    if (total == 0) {
        printf("Koi record nahi mila!\n");
        return;
    }

    for (int i = 0; i < total; i++) {
        printf("\n===============================\n");
        printf("ID: %d\n", db[i].studentID);
        printf("Name: %s\n", db[i].fullName);
        printf("Batch: %s\n", db[i].batch);
        printf("Membership: %s\n", db[i].membership);
        printf("Registered: %s\n", db[i].regDate);
        printf("DOB: %s\n", db[i].dob);
        printf("Interest: %s\n", db[i].interest);
    }
}

int main() {
    const char *file = "members.dat";
    loadDatabase(file);

    int option;
    while (1) {
        printf("\n===== Membership Manager =====\n");
        printf("1. Register Student\n");
        printf("2. Update Student\n");
        printf("3. Delete Student\n");
        printf("4. Show All Students\n");
        printf("5. Exit\n");
        printf("Choose option: ");
        scanf("%d", &option);

        if (option == 1) {
            Student s;
            printf("Student ID: ");
			scanf("%d", &s.studentID);
            printf("Full Name: "); 
			scanf("%s", s.fullName);
            printf("Batch: "); 
			scanf("%s", s.batch);
            printf("Membership (IEEE/ACM): "); 
			scanf("%s", s.membership);
            printf("Registration Date (YYYY-MM-DD): "); 
			scanf("%s", s.regDate);
            printf("DOB (YYYY-MM-DD): "); 
			scanf("%s", s.dob);
            printf("Interest (IEEE/ACM/Both): "); 
			scanf("%s", s.interest);
            addStudent(s, file);
        }
        else if (option == 2) {
            int id;
            printf("Student ID to update: ");
            scanf("%d", &id);
            updateStudent(id, file);
        }
        else if (option == 3) {
            int id;
            printf("Student ID to delete: ");
            scanf("%d", &id);
            deleteStudent(id, file);
        }
        else if (option == 4) {
            showAll();
        }
        else if (option == 5) {
            saveDatabase(file);
            printf("Program exit\n");
            break;
        }
        else {
            printf("Wrong option opted!\n");
        }
    }

    free(db);
    return 0;
}

