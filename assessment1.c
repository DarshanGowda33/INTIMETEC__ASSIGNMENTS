#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void removeSpace(char *exp){
    int index = 0, i = 0;
    for(i=0; i<strlen(exp); i++){
        if(exp[i] != ' '){
            exp[index] = exp[i];
            index++;
        }
    }
    exp[index]='\0';
}

int checkValid(char *exp){
    int i = 0;
    for(i=0; i<strlen(exp); i++){
        if(! (exp[i]>='0' && exp[i]<='9' || exp[i]=='+' || exp[i]=='-' || exp[i]=='*' || exp[i]=='/')){
            return 0;
        }
    }
    // if(!isdigit(i-1)){
    //     return 0;
    // }
    return 1;
}

int processExp(char *exp, char op[], int nums[]){
    int nIndex=0, oIndex=0, i=0;
    while(i < strlen(exp)){
        if(isdigit(exp[i])){
            int val=0;
            while(i<strlen(exp) && isdigit(exp[i])){
                val = val*10 + (exp[i++]-'0');
            }
            nums[nIndex++] = val;
        }else{
            op[oIndex++] = exp[i++];
        }
    }
    op[oIndex] = '\0';
    return nIndex;
}

int divMultiOp(int nums[], char ops[]) {
    int tempN[250], tempO[250];
    int nIndex = 0, oIndex = 0;
    tempN[nIndex++] = nums[0];
    for (int i = 0; ops[i]; i++) {
        if (ops[i] == '*') {
            tempN[nIndex - 1] *= nums[i + 1];
        } else if(ops[i] == '/') {
            if (nums[i + 1] == 0) {
                printf("Error: Division by zero\n");
                return 0;
            }
            tempN[nIndex - 1] /= nums[i + 1];
        } else {
            tempO[oIndex++] = ops[i];
            tempN[nIndex++] = nums[i + 1];
        }
    }
    for (int i = 0; i < nIndex; i++) 
        nums[i] = tempN[i];
    for (int i = 0; i < oIndex; i++) 
        ops[i] = tempO[i];
    ops[oIndex] = '\0';
    return 1;
}

int addSubOp(int nums[], char op[]){
    int result = nums[0]; 
    for(int i=0; op[i]; i++){
        if(op[i]=='+'){
            result += nums[i+1];
        }else{
            result -= nums[i-1];
        }
    }
    return result;
}

int main(){

    char exp[500], operator[250];
    int nums[500];
    printf("Enter The Expression : ");
    fgets(exp, sizeof(exp), stdin);
    exp[strcspn(exp,"\n")]='\0';

    removeSpace(exp);

    if(!checkValid(exp)){
        printf("Error: Invalid expression.");
        return 0;
    }

    int len = processExp(exp, operator, nums);

    if(!divMultiOp(nums,operator)) 
        return 0;

    int result = addSubOp(nums, operator);

    printf("%d",result);
    return 1;
}