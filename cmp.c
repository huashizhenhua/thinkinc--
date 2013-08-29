#include <stdio.h>

int main(int argc, char* argv[]){
    char* str1 = "hello world";
    char* str2 = "hello world";
    printf("cmp result = %d\n", strcmp(str1, str2));
    printf("cmp result = %d\n", strcmp(str1, "hello world"));
}
