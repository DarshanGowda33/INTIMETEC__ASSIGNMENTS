#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void removeSpace(char *expression){
    int index = 0, i = 0, expressionLength = strlen(expression);
    for(i=0; i<expressionLength; i++){
        if(expression[i] != ' '){
           expression[index] = expression[i];
            index++;
        }
    }
    expression[index]='\0';
}

int isValidOperator(char operator){
    return (operator == '+' || operator == '-' || operator == '*' || operator == '/');
}

void processExpression(char *expression, char operators[], int operands[]){
    int operandIndex=0, operatorsIndex=0, i=0, expressionLength = strlen(expression);
    while(i < expressionLength){
        if(isdigit(expression[i])){
            int value =0;
            while(i<expressionLength && isdigit(expression[i])){
                value = value*10 + (expression[i++]-'0');
            }
            operands[operandIndex++] = value;
        }else{
            operators[operatorsIndex++] = expression[i++];
        }
    }
    operators[operatorsIndex] = '\0';
}

int divisionMultiplicationOperation(int operands[], char operators[]) {
    int tempOperand[250], tempOperator[250];
    int operandIndex = 0, operatorIndex = 0,result = 1;
    tempOperand[operandIndex++] = operands[0];
    for (int i = 0; operators[i]; i++) {
        if (operators[i] == '*') {
            tempOperand[operandIndex - 1] *= operands[i + 1];
        } else if(operators[i] == '/') {
            if (operands[i + 1] == 0) {
                printf("Error: Division by zero\n");
                result = 0;
                break;
            }
            tempOperand[operandIndex - 1] /= operands[i + 1];
        } else {
            tempOperator[operatorIndex++] = operators[i];
            tempOperand[operandIndex++] = operands[i + 1];
        }
    }
    if(result){
        for (int i = 0; i < operandIndex; i++) 
            operands[i] = tempOperand[i];
        for (int i = 0; i < operatorIndex; i++) 
            operators[i] = tempOperator[i];
        operators[operatorIndex] = '\0';
    }
    return result;
}

int additionSubtractionOperation(int operands[], char operator[]){
    int result = operands[0]; 
    for(int i=0; operator[i]; i++){
        if(operator[i]=='+'){
            result += operands[i+1];
        }else{
            result -= operands[i+1];
        }
    }
    return result;
}

int main(){

    char expression[500], operator[250];
    int operands[500],valid = 1,success = 1;
    printf("Enter The Expression : ");
    fgets(expression, sizeof(expression), stdin);
    expression[strcspn(expression,"\n")]='\0';

    removeSpace(expression);

    int expressionLength = strlen(expression);

    for(int i=0; i<expressionLength; i++){
        if(!(isdigit(expression[i]) || isValidOperator(expression[i]))){
            printf("Error: Invalid expression.");
            valid = 0;
            break;
        }
    }
     
    if(valid){
        processExpression(expression, operator, operands);

        if(!divisionMultiplicationOperation(operands,operator)) {
            success = 0;
        }else{
            int result = additionSubtractionOperation(operands, operator);
            printf("%d\n",result);
        }
    }else{
        success = 0;
    }
    return success;
}
