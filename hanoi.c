#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

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

void hanoi(int n, char a, char b, char c) {

    if (n <= 0) {
        return;
    }
    hanoi(n - 1, a, c, b);
    printf("Disc %d: %c -> %c\n", n, a, b);
    hanoi(n - 1, c, b, a);

}

int main() {

    srand(time(NULL));

    if (compare_passwords()) {

        printf("Password is correct!\n");
        create_key();  
        while (1) {
            printf("Enter number of discs: ");
            int n;
            scanf("%d", &n);
            if (n <= 0) {
                return 0;
            } else {
                hanoi(n, 'a', 'b', 'c');
            }
        }
    } else {
        printf("Password is incorrect!\n");
    }

}