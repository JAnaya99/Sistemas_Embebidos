#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getPerimeter(char *a, char *b) {
    return 2 * atoi(a) + 2 * atoi(b);
}

int getArea(char *a, char *b) {
    return atoi(a) * atoi(b);
}

int main(int argc, char *argv[]) {

    if (argc != 6) {
        printf("We expected 6 arguments \n");
        return 0;
    }

    if (strcmp(argv[1], "-p") == 0) {
        printf("perimeter = %d units\n", getPerimeter(argv[3], argv[5]));
    } else if (strcmp(argv[1], "-a") == 0) {
        printf("area = %d units\n", getArea(argv[3], argv[5]));
    } else {
        printf("Wrong flag. Expected -p or -a \n");
    }

    return 0;
}
