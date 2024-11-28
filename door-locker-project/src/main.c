#include <stdio.h>
#include <stdlib.h>
#include "action_functions.h"
#include "validation_functions.h"

int main(int argc, char * argv[]) {
    int check;
    int *in0 = malloc(sizeof(int) * 4);
    int *in1 = malloc(sizeof(int) * 4);

    if (argc == 3) {
        check = validate(argv);
        if (check == 0) {
            puts("\nChecking values");
            check = fnchck(&in1, &in0);
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
