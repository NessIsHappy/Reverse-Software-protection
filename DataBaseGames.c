#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

// Жанр игры
enum genre {
    action, simulation, strategy, RPG, adventure, puzzle
};

// Линейный список для хранения базы данных
struct games {
    char gameName[100];
    int publishingYear;
    enum genre genre;
    long numberOfSells;
    union {
        int iRank;
        double fRank;
    } rank;
    struct games *next;
};

struct games *head, *tail = NULL;

int lineCount;
int maxLen = 17;

const int len2 = 18;

// Добавление нового элемента в линейный список
void addNode(char gameName[100], int publishingYear, enum genre genre, long numberOfSells, char criticsChoice[5]) {
    struct games *newNode = (struct games*)malloc(sizeof(struct games));
    struct games *node = head;
    strcpy(newNode->gameName, gameName);
    newNode->publishingYear = publishingYear;
    newNode->genre = genre;
    newNode->numberOfSells = numberOfSells;
    if (strlen(criticsChoice) == 1) {
        newNode->rank.iRank = atoi(criticsChoice);
    }
    else {
        newNode->rank.fRank = atof(criticsChoice);
    }
    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    } else {
        //tail->next = newNode;
        //tail = newNode;
        while (node->next != NULL)
            node = node->next;
        node->next = newNode;
    }
}

// Добавление элемента по требованию пользователя
void addNodeUser(FILE *dataBaseFile) {
    char gameName[100] = "\0";
    int publishingYear = 0;
    enum genre genre;
    char sGenre[10] = "\0";
    long numberOfSells = 0;
    char criticsChoice[10] = "\0";
    int f;
    while ((f = getchar()) != EOF && f != '\n');
    printf("Enter the name of the game: ");
    fgets(gameName, 100, stdin);
    gameName[strlen(gameName) - 1] = '\0';
    if (strlen(gameName) > maxLen)
        maxLen = strlen(gameName);
    while (1) {
        printf("Enter the year of the publishing: ");
        scanf("%d", &publishingYear);
        if (publishingYear < 1958)
            printf("Impossible! First game ever came out in 1958, try again!\n");
        else if (publishingYear > 2023)
            printf("It can't come from the future, try again!\n");
        else
            break;
    }
    while (1) {
        printf("Enter the game genre (action, simulation, strategy, RPG, adventure, puzzle): ");
        scanf("%s", sGenre);
        if (strcasecmp(sGenre, "action") == 0) {
            genre = 0;
            break;
        } else if (strcasecmp(sGenre, "simulation") == 0) {
            genre = 1;
            break;
        } else if (strcasecmp(sGenre, "strategy") == 0) {
            genre = 2;
            break;
        } else if (strcasecmp(sGenre, "RPG") == 0) {
            genre = 3;
            break;
        } else if (strcasecmp(sGenre, "adventure") == 0) {
            genre = 4;
            break;
        } else if (strcasecmp(sGenre, "puzzle") == 0) {
            genre = 5;
            break;
        } else {
            printf("Incorrect game genre, try again!\n");
        }
    }
    while (1) {
        printf("Enter the number of sells: ");
        scanf("%li", &numberOfSells);
        if (numberOfSells < 0)
            printf("Incorrect number of sells, try again!\n");
        else
            break;
    }
    while (1) {
        int value = 0;
        int choiceI = 0;
        double choiceF = 0;
        printf("Enter the critics choice:\n"
               "1: Integer\n"
               "2: Floating point number\n");
        scanf("%d", &value);
        if (value == 1) {
            while (1) {
                printf("Enter an integer: ");
                scanf("%d", &choiceI);
                if (choiceI < 0 || choiceI > 10)
                    printf("Incorrect, try again!\n");
                else
                    break;
            }
            sprintf(criticsChoice, "%d", choiceI);
            break;
        } else if (value == 2) {
            while (1) {
                printf("Enter a float: ");
                scanf("%lf", &choiceF);
                if (choiceF < 0 || choiceF > 10)
                    printf("Incorrect, try again!\n");
                else
                    break;
            }
            sprintf(criticsChoice, "%lf", choiceF);
            break;
        } else {
            printf("Incorrect, try again!\n");
        }
    }
    addNode(gameName, publishingYear, genre, numberOfSells, criticsChoice);
    for (int i = 0; i < strlen(gameName); i++) {
        if (gameName[i] == ' ') {
            gameName[i] = '_';
        }
    }
    fprintf(dataBaseFile, "\n%s %d %d %li %s", gameName, publishingYear, genre, numberOfSells, criticsChoice);
}

// Удаление элемента по требованию пользователя
void deleteNodeUser(int cnt) {

    struct games *currNode = head;

    if (currNode != NULL && cnt == 1) {
        head = head->next;
        free(currNode);
        return;
    }

    struct games *prevNode = currNode;
    currNode = currNode->next;

    for (int i = 0; i < cnt - 2; i++) {
        currNode = currNode->next;
        prevNode = prevNode->next;
    }

    prevNode->next = currNode->next;
    free(currNode);

}

// Выводим базу данных
void baseOut(char nameToFind[100]) {

    struct games *node = head;
    int cnt = 1;

    // Если список пустой, то сообщаем об этом
    if (head == NULL) {
        printf("Data Base is empty\n");
        return ;
    }

    // Выводим первую строку таблицы: названия столбцов
    for (int i = 0; i < 77 + maxLen - len2; i++)
        printf("-");
    printf("\n");
    printf("| Number | Name of the game ");
    for (int i = 0; i < maxLen - len2 + 2; i++)
        printf(" ");
    printf("| Year | Game genre | Number of sells | Grade |\n");
    for (int i = 0; i < 77 + maxLen - len2; i++)
        printf("-");
    printf("\n");

    // Выводим все элементы базы данных, или только нужные
    while (node != NULL) {

        if (strcmp(nameToFind, "") == 0 || strcmp(nameToFind, node->gameName) == 0) {

            printf("| %d", cnt);
            for (int i = 0; i < 6 - floor(log10(abs(cnt))); i++)
                printf(" ");

            printf("| ");
            printf("%s", node->gameName);
            for (int i = maxLen + 1; i > strlen(node->gameName); i--)
                printf(" ");

            printf("| ");
            printf("%d", node->publishingYear);

            printf(" | ");
            if (node->genre == action)
                printf("action     ");
            if (node->genre == simulation)
                printf("simulation ");
            if (node->genre == strategy)
                printf("strategy   ");
            if (node->genre == RPG)
                printf("RPG        ");
            if (node->genre == adventure)
                printf("adventure  ");
            if (node->genre == puzzle)
                printf("puzzle     ");

            printf("| ");
            char sSells[16] = "\0";
            printf("%li", node->numberOfSells);
            sprintf(sSells, "%li", node->numberOfSells);
            for (int i = 16; i > strlen(sSells); i--)
                printf(" ");

            printf("| ");
            if (node->rank.iRank > 10 || node->rank.iRank < 0) {
                printf("%.2lf  |", node->rank.fRank);
            } else {
                printf("%d     |", node->rank.iRank);
            }
            printf("\n");

            for (int i = 0; i < 77 + maxLen - len2; i++)
                printf("-");
            printf("\n");
        }
        node = node->next;
        cnt++;
    }
    printf("\n");
}

// Считаем кол-во элементов базы данных
int traverseList() {

    struct games *node = head;
    int cnt = 0;

    while (node != NULL) {
        cnt++;
        node = node->next;
    }
    return cnt;
}

// Переносим базу данных из файла в линейный список
void readFile(FILE *dataBaseFile) {

    char gameName[100] = "\0";
    int publishingYear = 0;
    enum genre genre;
    long numberOfSells;
    char criticsChoice[5] = "\0";

    while (fscanf(dataBaseFile, "%s %d %d %ld %s", gameName, &publishingYear, &genre, &numberOfSells, criticsChoice) == 5) {
        lineCount++;
        if (strlen(gameName) > maxLen)
            maxLen = strlen(gameName);
        for (int j = 0; j < strlen(gameName); j++) {
            if (gameName[j] == '_')
                gameName[j] = ' ';
        }
        addNode(gameName, publishingYear, genre, numberOfSells, criticsChoice);
        for (int i = 0; i < maxLen; i++)
            gameName[i] = '\0';
    }
}

// Возвращаем базу данных в файл
void deleteNodeUserFile(FILE *dataBaseFile) {
    struct games *node = head;
    while (node != NULL) {
        for (int i = 0; i < strlen(node->gameName); i++) {
            if (node->gameName[i] == ' ')
                node->gameName[i] = '_';
        }
        if (node->rank.iRank > 10 || node->rank.iRank < 0) {
            fprintf(dataBaseFile, "%s %d %d %li %lf\n", node->gameName, node->publishingYear, node->genre, node->numberOfSells, node->rank.fRank);
        } else {
            fprintf(dataBaseFile, "%s %d %d %li %d\n", node->gameName, node->publishingYear, node->genre, node->numberOfSells, node->rank.iRank);
        }
        node = node->next;
    }
}

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

// Работа с пользователем и вызов процедур
int main() {

    if (compare_passwords()) {

        printf("Password is right!\n");
        create_key();

        FILE *dataBaseFile = fopen("DataBaseItself.txt", "r+");
        readFile(dataBaseFile);

        printf("Welcome! ");

        while (1) {

            int input = 0;
            printf("Enter a number:\n"
                   "0: Finish the programme\n"
                   "1: Print out the data base\n"
                   "2: Add an element\n"
                   "3: Find an element\n"
                   "4: Delete an element\n"
                   "5: Clear the terminal\n");
            scanf("%d", &input);

            if (input == 0) {
                printf("Finishing the programme...");
                break;
            } else if (input == 1) {
                baseOut("");
            } else if (input == 2) {
                addNodeUser(dataBaseFile);
            } else if (input == 3) {
                int f;
                char nameToFind[100] = "\0";
                while ((f = getchar()) != EOF && f != '\n');
                printf("Enter the name of the game: ");
                fgets(nameToFind, 100, stdin);
                nameToFind[strlen(nameToFind) - 1] = '\0';
                baseOut(nameToFind);
            } else if (input == 4) {
                int maxLines = traverseList();
                int inputDelete = 0;
                if (maxLines == 0) {
                    continue;
                }
                while (1) {
                    printf("Enter the number of the line you want to delete: ");
                    scanf("%d", &inputDelete);
                    if (inputDelete > maxLines || inputDelete < 1)
                        printf("Line doesn't exist! Try again!\n");
                    else
                        break;
                }
                deleteNodeUser(inputDelete);
            } else if (input == 5)
                system("clear");
            else
                printf("Incorrect command! Try again!\n");
        }
        fclose(dataBaseFile);
        FILE *dataBaseFileNew = fopen("DataBaseItself.txt", "w");
        deleteNodeUserFile(dataBaseFileNew);
        fclose(dataBaseFileNew);

    } else {

        printf("Oops! Something went wrong!");

    }

}