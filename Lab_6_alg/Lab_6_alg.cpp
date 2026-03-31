#include "Header.h"
#include <time.h>
#include <stdlib.h>

MenuItem menu[] = {
    {"Add Question", addQuestion},
    {"Print Database", printDatabase},
    {"Search Question", searchQuestion},
    {"Delete Question", deleteQuestion},
    {"Group by Subject", groupQuestionsBySubject},
    {"Start Testing", testing},
    {"Total Test", totalTesting},
    {"Custom Test", customTesting}, // Додано сюди
    {"Exit", exitProgram}
};

int main(int argc, char** argv) {
    srand((unsigned int)time(NULL));

    // Використання параметрів командного рядка
    readFilePath(argc, argv);

    int choice;
    int menuSize = sizeof(menu) / sizeof(MenuItem);

    while (1) {
        printf("\n--- MENU ---\n");
        for (int i = 0; i < menuSize; i++) printf("%d. %s\n", i+1, menu[i].name);
        printf("Choice: ");
        if (scanf_s("%d", &choice) == 1 && choice >= 0 && choice-1 < menuSize) {
            menu[choice-1].func();
        }
        else {
            rewind(stdin);
        }
    }
    return 0;
}