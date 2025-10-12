#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100
#define FILE_PATH "C:\\Users\\darshan gowda k r\\Documents\\User Data\\users.txt"

typedef struct 
{
    int id;
    char name[MAX_NAME_LEN];
    int age;
} User;

typedef enum 
{
    CREATE_USER = 1,
    READ_USERS,
    UPDATE_USER,
    DELETE_USER,
    EXIT
} MenuOption;

FILE* openFile(const char *path, const char *mode) 
{
    FILE *fp = fopen(path, mode);
    if (!fp) 
    {
        perror("Unable to open file");
    }
    return fp;
}

void closeFile(FILE *fp) 
{
    if (fp != NULL) 
    {
        fclose(fp);
    }
}

void createUser() 
{
    FILE *fp = openFile(FILE_PATH, "a");
    if (fp) {
        User user;
        printf("Enter ID: ");
        scanf("%d", &user.id);
        printf("Enter Name: ");
        scanf(" %[^\n]", user.name);
        printf("Enter Age: ");
        scanf("%d", &user.age);

        fprintf(fp, "%d,%s,%d\n", user.id, user.name, user.age);
        closeFile(fp);
        printf("User added successfully.\n");
    }

}


void readUsers() 
{
    FILE *fp = openFile(FILE_PATH, "r");
    if (fp) 
    {
        User user;
        char line[256];
        printf("\nUser List\n");
        while (fgets(line, sizeof(line), fp)) 
        {
            sscanf(line, "%d,%[^,],%d", &user.id, user.name, &user.age);
            printf("ID: %d | Name: %s | Age: %d\n", user.id, user.name, user.age);
        }
        closeFile(fp);
    }
}

void updateUser() 
{
    int targetId;
    printf("Enter ID of user to update: ");
    scanf("%d", &targetId);

    FILE *fp = openFile(FILE_PATH, "r");
    FILE *temp = openFile("temp.txt", "w");
    if (fp && temp) 
    {
        User user;
        char line[256];
        int found = 0;

        while (fgets(line, sizeof(line), fp)) 
        {
            sscanf(line, "%d,%[^,],%d", &user.id, user.name, &user.age);
            if (user.id == targetId) 
            {
                found = 1;
                printf("Enter new Name: ");
                scanf(" %[^\n]", user.name);
                printf("Enter new Age: ");
                scanf("%d", &user.age);
            }
            fprintf(temp, "%d,%s,%d\n", user.id, user.name, user.age);
        }

        closeFile(fp);
        closeFile(temp);
        remove(FILE_PATH);
        rename("temp.txt",FILE_PATH);

        if (found)
        {
            printf("User updated successfully.\n");
        }
        else
        {
            printf("User with ID %d not found.\n", targetId);
        }
    }
    else
    {
        perror("Unable to open file");
    }
}

void deleteUser() 
{
    int targetId;
    printf("Enter ID of user to delete: ");
    scanf("%d", &targetId);

    FILE *fp = openFile(FILE_PATH, "r");
    FILE *temp = openFile("temp.txt", "w");
    if (fp && temp) 
    {
        User user;
        char line[256];
        int found = 0;

        while (fgets(line, sizeof(line), fp)) 
        {
            sscanf(line, "%d,%[^,],%d", &user.id, user.name, &user.age);
            if (user.id != targetId) 
            {
                fprintf(temp, "%d,%s,%d\n", user.id, user.name, user.age);
            } 
            else 
            {
                found = 1;
            }
        }

        closeFile(fp);
        closeFile(temp);
        remove(FILE_PATH);
        rename("temp.txt", FILE_PATH);

        if (found)
        {
            printf("User deleted successfully.\n");
        }
        else
        {
            printf("User with ID %d not found.\n", targetId);
        }
    }
    else
    {
        perror("Unable to open file");
    }    
}

int main() 
{
    int choice;
    do 
    {
        printf("\nUser Management\n");
        printf("%d. Create User\n", CREATE_USER);
        printf("%d. Read Users\n", READ_USERS);
        printf("%d. Update User\n", UPDATE_USER);
        printf("%d. Delete User\n", DELETE_USER);
        printf("%d. Exit\n", EXIT);
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch ((MenuOption)choice) 
        {
            case CREATE_USER:
                createUser();
                break;
            case READ_USERS:
                readUsers();
                break;
            case UPDATE_USER:
                updateUser();
                break;
            case DELETE_USER:
                deleteUser();
                break;
            case EXIT:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } 
    while (choice != EXIT);

    return 0;
}
