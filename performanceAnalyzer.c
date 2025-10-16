#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100

struct studentDetails
{
    int roll_num;
    char name[50];
    int marks[3];
    int total;
    float average_marks;
    char grade;
};

void computeGrade(struct studentDetails *student)
{
    if (student->average_marks >= 85)
    {
        student->grade = 'A';
    }
    else  if (student->average_marks >= 70)
    {
        student->grade = 'B';
    }
    else  if (student->average_marks >= 50)
    {
        student->grade = 'C';
    }
    else  if (student->average_marks >= 35)
    {
        student->grade = 'D';
    }
    else
    {
        student->grade = 'F';
    }
}

void computePerformance(char grade)
{
    int countOfStars = 0;
    switch(grade)
    {
        case 'A': 
            countOfStars = 5;
            break;
        case 'B': 
            countOfStars = 4;
            break;
        case 'C': 
            countOfStars = 3;
            break;
        case 'D': 
            countOfStars = 2;
            break;
        default:
            break;
    }
    printf("Performance: ");
    for(int index = 0; index < countOfStars; index++)
    {
        printf("*");
    }
    printf("\n");
}

void printRoll(struct studentDetails student[],int studentsCount,int index)
{
    if (index == studentsCount)
    {
        return;
    }
    printf(" %d",student[index].roll_num);
    printRoll(student,studentsCount,index+1);
}

void printStudentDetails(struct studentDetails *students,int studentsCount)
{
    printf("\nStudent details: \n");
    for (int index = 0; index < studentsCount; index++)
    {
        printf("\nRoll : %d\n",students[index].roll_num);
        printf("Name : %s\n",students[index].name);
        printf("Total Marks : %d\n",students[index].total);
        printf("Average Marks : %.2f\n",students[index].average_marks);
        printf("Grade : %c\n",students[index].grade);
        
        if (students[index].grade == 'F')
        {
            continue;
        }
        else
        {
            computePerformance(students[index].grade);
        }
    }
}

int main()
{
    struct studentDetails students[MAX_STUDENTS];
    int studentsCount,index;
    printf("\nEnter number of students: ");
    scanf("%d",&studentsCount);
    for (int index = 0; index < studentsCount; index++)
    {
        printf("Enter student %d details(roll,name,marks1,marks2,marks3): ",index+1);
        scanf("%d %s %d %d %d",&students[index].roll_num,students[index].name,&students[index].marks[0],&students[index].marks[1],&students[index].marks[2]);
        students[index].total = students[index].marks[0]+students[index].marks[1]+students[index].marks[2];
        students[index].average_marks = students[index].total / 3.0;
        computeGrade(&students[index]);
    }
    printStudentDetails(students,studentsCount);
    printf("\nList of Roll Numbers (via recursion): ");
    printRoll(students,studentsCount,0);
    printf("\n");
    return 0;
}
