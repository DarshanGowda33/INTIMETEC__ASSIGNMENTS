#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    int age;
} User;

void createUser() {
    FILE *fp = fopen("C:\\Users\\darshan gowda k r\\Documents\\User Data\\users.txt", "a");
    if (!fp) {
        perror("Unable to open file");
        return;
    }

    User user;
    printf("Enter ID: ");
    scanf("%d", &user.id);
    printf("Enter Name: ");
    scanf(" %[^\n]", user.name);
    printf("Enter Age: ");
    scanf("%d", &user.age);

    fprintf(fp, "%d,%s,%d\n", user.id, user.name, user.age);
    fclose(fp);
    printf("User added successfully.\n");
}

void readUsers() {
    FILE *fp = fopen("C:\\Users\\darshan gowda k r\\Documents\\User Data\\users.txt", "r");
    if (!fp) {
        perror("Unable to open file");
        return;
    }

    User user;
    char line[256];
    printf("\n--- User List ---\n");
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,%[^,],%d", &user.id, user.name, &user.age);
        printf("ID: %d | Name: %s | Age: %d\n", user.id, user.name, user.age);
    }
    fclose(fp);
}

void updateUser() {
    int targetId;
    printf("Enter ID of user to update: ");
    scanf("%d", &targetId);

    FILE *fp = fopen("C:\\Users\\darshan gowda k r\\Documents\\User Data\\users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        perror("Unable to open file");
        return;
    }

    User user;
    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,%[^,],%d", &user.id, user.name, &user.age);
        if (user.id == targetId) {
            found = 1;
            printf("Enter new Name: ");
            scanf(" %[^\n]", user.name);
            printf("Enter new Age: ");
            scanf("%d", &user.age);
        }
        fprintf(temp, "%d,%s,%d\n", user.id, user.name, user.age);
    }

    fclose(fp);
    fclose(temp);
    remove("C:\\Users\\darshan gowda k r\\Documents\\User Data\\users.txt");
    rename("temp.txt", "C:\\Users\\darshan gowda k r\\Documents\\User Data\\users.txt");

    if (found)
        printf("User updated successfully.\n");
    else
        printf("User with ID %d not found.\n", targetId);
}

void deleteUser() {
    int targetId;
    printf("Enter ID of user to delete: ");
    scanf("%d", &targetId);

    FILE *fp = fopen("C:\\Users\\darshan gowda k r\\Documents\\User Data\\users.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
        perror("Unable to open file");
        return;
    }

    User user;
    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d,%[^,],%d", &user.id, user.name, &user.age);
        if (user.id != targetId) {
            fprintf(temp, "%d,%s,%d\n", user.id, user.name, user.age);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);
    remove("C:\\Users\\darshan gowda k r\\Documents\\User Data\\users.txt");
    rename("temp.txt", "C:\\Users\\darshan gowda k r\\Documents\\User Data\\users.txt");

    if (found)
        printf("User deleted successfully.\n");
    else
        printf("User with ID %d not found.\n", targetId);
}

int main() {
    int choice;
    do {
        printf("\n--- User Management ---\n");
        printf("1. Create User\n");
        printf("2. Read Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createUser(); break;
            case 2: readUsers(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);

    return 0;
}