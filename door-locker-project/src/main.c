#include <stdio.h>
#include <stdlib.h>

#include "action_functions.h"
#include "validation_functions.h"

int is_numeric(const char *str) {
    if (str == NULL || *str == '\0') {
        return 0;
    }

    for (size_t i = 0; i < strlen(str); i++) {
        if (!isdigit((unsigned char) str[i])) {
            return 0; 
        }
    }
    return 1; 
}

int main(int argc, char * argv[]) {
    int check;
    int in0;
    int in1;

    if (argc == 3) {
        if (!is_numeric(argv[1]) || !is_numeric(argv[2])) {
            puts("Error: Both arguments must be numeric values.");
            return 1;
        }
        check = validate(argv);
        if (check == 0) {
            puts("\nChecking values");
            in0   = atoi(argv[2]);
            in1   = atoi(argv[1]);
            check = fnchck(in1, in0);
            if (check == 0xf) {
                fngrt();
            } else {
                fnr();
            }
        } else {
            fnr();
        }
        check = 0;
    } else {
        puts("Usage : client <chiffre0> <chiffre1>");
        check = 1;
    }
    return check;
}
