#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int id;
    char name[50];
    char course[50];
    char year[20];
    char address[100];
};

struct Complaint {
    int student_id;
    char message[200];
};

// FUNCTION DECLARATIONS
void adminMenu();
void studentMenu(int stID);
void addStudent();
void viewStudents();
void modifyStudent();
void deleteStudent();
void raiseComplaint(int stID);
void viewComplaints();
void viewOwnRecord(int stID);

// ------------------ MAIN MENU ------------------
int main() {
    int choice, stID;

    while (1) {
        printf("\n========= STUDENT RECORD MANAGEMENT SYSTEM =========\n");
        printf("1. Admin Login\n");
        printf("2. Student Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                adminMenu();
                break;

            case 2:
                printf("Enter Student ID: ");
                scanf("%d", &stID);
                studentMenu(stID);
                break;

            case 3:
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}

// ------------------ ADMIN MENU ------------------
void adminMenu() {
    int choice;

    while (1) {
        printf("\n=========== ADMIN MENU ===========\n");
        printf("1. Add Student Record\n");
        printf("2. View All Records\n");
        printf("3. Modify Student Record\n");
        printf("4. Delete Student Record\n");
        printf("5. View Complaints\n");
        printf("6. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: modifyStudent(); break;
            case 4: deleteStudent(); break;
            case 5: viewComplaints(); break;
            case 6: return;
            default: printf("Invalid choice!\n");
        }
    }
}

// ------------------ STUDENT MENU ------------------
void studentMenu(int stID) {
    int choice;

    while (1) {
        printf("\n=========== STUDENT MENU ===========\n");
        printf("1. View My Record\n");
        printf("2. Raise Complaint\n");
        printf("3. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: viewOwnRecord(stID); break;
            case 2: raiseComplaint(stID); break;
            case 3: return;
            default: printf("Invalid choice!\n");
        }
    }
}

// ------------------ ADD STUDENT ------------------
void addStudent() {
    struct Student s;
    FILE *fp = fopen("students.txt", "ab+");  // create file if not exists

    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter ID: ");
    scanf("%d", &s.id);
    printf("Enter Name: ");
    scanf(" %[^\n]s", s.name);
    printf("Enter Course: ");
    scanf(" %[^\n]s", s.course);
    printf("Enter Year: ");
    scanf(" %[^\n]s", s.year);
    printf("Enter Address: ");
    scanf(" %[^\n]s", s.address);

    fwrite(&s, sizeof(s), 1, fp);
    fclose(fp);

    printf("Record added successfully!\n");
}

// ------------------ VIEW ALL STUDENTS ------------------
void viewStudents() {
    struct Student s;
    FILE *fp = fopen("students.txt", "rb");

    if (!fp) {
        printf("No student records found!\n");
        return;
    }

    printf("\n========= ALL STUDENT RECORDS =========\n");

    while (fread(&s, sizeof(s), 1, fp)) {
        printf("\nID: %d\nName: %s\nCourse: %s\nYear: %s\nAddress: %s\n",
               s.id, s.name, s.course, s.year, s.address);
    }

    fclose(fp);
}

// ------------------ MODIFY STUDENT ------------------
void modifyStudent() {
    struct Student s;
    FILE *fp = fopen("students.txt", "rb");

    if (!fp) {
        printf("No student records found!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "wb");
    if (!temp) {
        printf("Error creating temp file!\n");
        fclose(fp);
        return;
    }

    int id, found = 0;
    printf("Enter ID to modify: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id == id) {
            found = 1;
            printf("Enter new name: ");
            scanf(" %[^\n]s", s.name);
            printf("Enter new course: ");
            scanf(" %[^\n]s", s.course);
            printf("Enter new year: ");
            scanf(" %[^\n]s", s.year);
            printf("Enter new address: ");
            scanf(" %[^\n]s", s.address);
        }
        fwrite(&s, sizeof(s), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("students.txt");
    rename("temp.txt", "students.txt");

    if (found)
        printf("Record updated successfully!\n");
    else
        printf("Record not found!\n");
}

// ------------------ DELETE STUDENT ------------------
void deleteStudent() {
    struct Student s;
    FILE *fp = fopen("students.txt", "rb");

    if (!fp) {
        printf("No student records found!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "wb");
    if (!temp) {
        printf("Error creating temp file!\n");
        fclose(fp);
        return;
    }

    int id, found = 0;
    printf("Enter ID to delete: ");
    scanf("%d", &id);

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id != id)
            fwrite(&s, sizeof(s), 1, temp);
        else
            found = 1;
    }

    fclose(fp);
    fclose(temp);

    remove("students.txt");
    rename("temp.txt", "students.txt");

    if (found)
        printf("Record deleted successfully!\n");
    else
        printf("Record not found!\n");
}

// ------------------ RAISE COMPLAINT ------------------
void raiseComplaint(int stID) {
    struct Complaint c;
    FILE *fp = fopen("complaints.txt", "ab+");  // create file if not exists

    if (!fp) {
        printf("Error opening complaints file!\n");
        return;
    }

    c.student_id = stID;
    printf("Enter your complaint: ");
    scanf(" %[^\n]s", c.message);

    fwrite(&c, sizeof(c), 1, fp);
    fclose(fp);

    printf("Complaint submitted successfully!\n");
}

// ------------------ VIEW COMPLAINTS ------------------
void viewComplaints() {
    struct Complaint c;
    FILE *fp = fopen("complaints.txt", "rb");

    if (!fp) {
        printf("No complaints found!\n");
        return;
    }

    printf("\n========= ALL COMPLAINTS =========\n");

    while (fread(&c, sizeof(c), 1, fp)) {
        printf("\nStudent ID: %d\nComplaint: %s\n",
               c.student_id, c.message);
    }

    fclose(fp);
}

// ------------------ VIEW OWN RECORD ------------------
void viewOwnRecord(int stID) {
    struct Student s;
    FILE *fp = fopen("students.txt", "rb");

    if (!fp) {
        printf("No student records found!\n");
        return;
    }

    while (fread(&s, sizeof(s), 1, fp)) {
        if (s.id == stID) {
            printf("\n========= YOUR RECORD =========\n");
            printf("ID: %d\nName: %s\nCourse: %s\nYear: %s\nAddress: %s\n",
                   s.id, s.name, s.course, s.year, s.address);
            fclose(fp);
            return;
        }
    }

    printf("Record not found!\n");
    fclose(fp);
}