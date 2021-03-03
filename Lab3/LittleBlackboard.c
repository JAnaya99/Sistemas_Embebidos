#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("We expected 3 arguments \n");
        return 0;
    }

    if (strcmp(argv[1], "-s") != 0) {
        printf("Wrong flag. Expected -s \n");
    }

    const int limit = atoi(argv[2]);
    if (limit < 1) {
        printf("Expected size > 0 \n");
    }

    // Reading names and ids.
    char a[limit][50];
    char id[50];
    int count = 0;
    char aux = 'Y';
    while (aux == 'Y' && count < limit) {
        printf("Enter Name: ");
        scanf("%s", &a[count]);
        printf("Enter ID: ");
        scanf("%s", &id);
        strcat(a[count], "\t \t");
        strcat(a[count], id);
        strcat(a[count], "\n");
        printf("Do you wish to add more [Y/n]: ");
        scanf("%c", &aux);
        scanf("%c", &aux);
        ++count;
    }

    // creating file pointer to work with files.
    FILE *fptr;

    // opening file in writing mode
    fptr = fopen("Datalog.txt", "w");

    // exiting program.
    if (fptr == NULL) {
        printf("Error!");
        exit(1);
    }

    // Writing on file.
    fprintf(fptr, "%s", "NAME \tID \n");
    for (int i = 0; i < count; ++i) {
        fprintf(fptr, "%s", a[i]);
    }

    fclose(fptr);


    return 0;
}


