#include <stdio.h>
#include <string.h>

#include "validation_functions.h"

int fnchck(int a, int b) {
    int check;

    if (b == a) {
        puts("Valid access.");
        check = 0xf;
    } else {
        check = 0;
    }
    return check;
}

int validate(char * argv[]) {
    char buffer[3];
    printf("You entered %s and %s. \nDo you agree ? (Y,n):\n", argv[1], argv[2]);
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        printf("You entered: %s", buffer);
        buffer[strcspn(buffer,"\n")]='\0';
        
    } else {
        printf("Error reading input.\n");
    }
    return strcmp(buffer, "Y") && strcmp(buffer, "y");
}
