#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <Windows.h>

char password[] = "WXYZ[\\'()*+,";
char file_1[] = "fWiimehZ$jnj";
char file_2[] = "i[h_Wb$jnj";
char error_msg_1[] = "L_hjkWbUCWY^_d[UZ[j[Yj[Z";
char error_msg_2[] = "DeUikY^U\\_b[";
char error_msg_3[] = "FWiimehZU_iUYehh[Yj";
char error_msg_4[] = "FWiimehZU_iU_dYehh[Yj";
char error_msg_5[] = "9H9U_iU_dYehh[Yj";
char msg_1[] = ";dj[hUdkcX[hUe\\UZ_iYi0";

int key = 10;
uint32_t crc = -194981038;

void decrypt(char *string) {

    for (int i = 0; i < strlen(string); i++) {
        string[i] += key;
    }

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

void detect_virtual_machine() {

    HKEY hTheKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Enum\\PCI\\VEN_5333&DEV_8811&SUBSYS_00000000&REV_00", 0, KEY_QUERY_VALUE, &hTheKey) == ERROR_SUCCESS) {
        RegCloseKey(hTheKey);
        decrypt(error_msg_1);
        printf("%s\n", error_msg_1);
        ExitProcess(0);
    }

}

void remote_debugger() {

    BOOL isDebugerPresent = FALSE;
    if (CheckRemoteDebuggerPresent(GetCurrentProcess(), &isDebugerPresent)) {
        if (isDebugerPresent) {
            ExitProcess(0);
        }
    }
}

void detect_ollydbg_or_windbg() {

    HANDLE hOlly = FindWindow(TEXT("OLLYDBG"), NULL);
    HANDLE hWinDbg = FindWindow(TEXT("WinDbgFrameClass"), NULL);
    if (hOlly || hWinDbg)
        ExitProcess(0);

}

void check_ticks() {
	int x = 0;
	DWORD Time = GetTickCount64();
	for (short i = 0; i < 10; i++) {
		x++;
	}
	Time = GetTickCount64()-Time;
	if (Time > 0x10) 
		ExitProcess(0);
}


bool compare_passwords() {

    FILE *password_file = fopen(file_1, "r");
    if (password_file == NULL) {
        printf("%s\n", error_msg_2);
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

    decrypt(file_2);
    FILE *serial = fopen(file_2, "w");

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

    /* Выявление виртаульной машины */
    detect_virtual_machine();

    /* Выявление отладчиков */
    if (IsDebuggerPresent())
        return 0;

    remote_debugger();

    detect_ollydbg_or_windbg();

    check_ticks();
    
    /* Проверка CRC */
    uint32_t expected_crc = crc32((const void *)compare_passwords, (size_t)((char *)main - (char *)compare_passwords));
    printf("%d\n", expected_crc);

    if (expected_crc != crc) {
        decrypt(error_msg_5);
        printf("%s\n", error_msg_5);
        return 0;
    }

    /* Расшифровка строк */
    decrypt(password);
    decrypt(file_1);
    decrypt(error_msg_2);

    if (compare_passwords()) {

        compare_passwords();
        decrypt(error_msg_3);
        printf("%s\n", error_msg_3);
        create_key();  
        decrypt(msg_1);
        while (1) {
            if (!compare_passwords())
                return 0;
            printf("%s", msg_1);
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
        decrypt(error_msg_4);
        printf("%s\n", error_msg_4);
    }

}