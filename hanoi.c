#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <Windows.h>

char password[] = "abcdef123456";
char key[] = "cloud";
uint32_t crc = 1922916129;

void decrypt() {
    int key_length = strlen(key);
    int cnt = 0;
    for (int i = 0; i < strlen(password); i++) {
        password[i] = (password[i]);
        cnt++;
        if (cnt == key_length)
            cnt = 0;
    }
    printf("%s\n", password);
}

uint32_t crc32(const void *data, size_t length) {
    uint32_t crc = 0xFFFFFFFF;
    const uint8_t *p = data;
    for (size_t i = 0; i < length; i++) {
        crc ^= p[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc ^ 0xFFFFFFFF;
}

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
    decrypt();
    uint32_t expected_crc = crc32((const void *)compare_passwords, (size_t)((char *)main - (char *)compare_passwords));
    printf("%d\n", expected_crc);

    if (expected_crc != crc) {
        printf("CRC is incorrect!\n");
        return 0;
    }

    if (compare_passwords()) {

        compare_passwords();
        printf("Password is correct!\n");
        create_key();  
        while (1) {
            printf("Enter number of discs: ");
            int n;
            scanf("%d", &n);
            if (n <= 0) {
                return 0;
            } else {
                compare_passwords();
                hanoi(n, 'a', 'b', 'c');
            }
        }
    } else {
        printf("Password is incorrect!\n");
    }

}