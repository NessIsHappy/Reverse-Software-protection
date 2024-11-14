#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

char password[] = "abcdef123456";

bool compare_passwords() {

    FILE *password_file = fopen("password.txt", "r");
    if (password_file == NULL) {
        printf("No such file!");
        return false;
    }

    char password_from_file[100] = {'\0'};
    fgets(password_from_file, 100, password_file);
    fclose(password_file);
    //printf("%s\n", password_from_file);

    if (!strcmp(password, password_from_file))
        return true;
    else
        return false;

}

char random_char() {

    int r = rand() % 36;
    if (r < 26) {
        return 'A' + r;
    } else {
        return '0' + (r - 26);
    }

}


void create_key() {

    FILE *serial = fopen("serial.txt", "w");

    char key[] = "KEY$xxxxxxxxxx$";
    for (int i = 0; i < 10; i++) {
        key[i + 4] = random_char();
    }
    fputs(key, serial);

}

int main(void) {

    srand(time(NULL));

    if (compare_passwords()) {
        create_key();
        printf("Everything is fine!");
    } else {
        printf("Oops! Something went wrong(");
    }

}
