#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100

struct studentDetails
{
    int roll_num;
    char name[50];
    int marks1;
    int marks2;
    int marks3;
    int total;
    float average_marks;
    char grade;
};

int computeTotalMarks(struct studentDetails student)
{
    return (student.marks1+student.marks2+student.marks3);
}

float computeAverageMarks(struct studentDetails student)
{
    return (student.marks1+student.marks2+student.marks3)/3.0;
}

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
    if (grade == 'A')
    {
        printf("Performance : *****\n");
    }
    else if (grade == 'B')
    {
        printf("Performance : ****\n");
    }
    else if (grade == 'C')
    {
        printf("Performance : ***\n");
    }
    else if (grade == 'D')
    {
        printf("Performance : **\n");
    }
}

void printRoll(struct studentDetails student[],int num,int index)
{
    if (index == num)
    {
        return;
    }
    printf(" %d",student[index].roll_num);
    printRoll(student,num,index+1);
}

void printStudentDetails(struct studentDetails *students,int num)
{
    printf("\nStudent details: \n");
    for (int index = 0; index < num; index++)
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
    int num,index;
    printf("\nEnter number of students: ");
    scanf("%d",&num);
    for (int index = 0; index < num; index++)
    {
        printf("Enter student %d details(roll,name,marks1,marks2,marks3): ",index+1);
        scanf("%d %s %d %d %d",&students[index].roll_num,students[index].name,&students[index].marks1,&students[index].marks2,&students[index].marks3);
        students[index].total = computeTotalMarks(students[index]);
        students[index].average_marks = computeAverageMarks(students[index]);
        computeGrade(&students[index]);
    }
    printStudentDetails(students,num);
    printf("\nList of Roll Numbers (via recursion): ");
    printRoll(students,num,0);
    printf("\n");
    return 0;
}
