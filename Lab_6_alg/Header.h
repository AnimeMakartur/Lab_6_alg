#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_OPT_TEXT 300
#define MAX_SUBJECTS 3
#define MAX_OPTIONS 4
#define MAX_QUESTIONS 20

#define isValidSubjectNum(subjectNum) ((subjectNum) >= 1 && (subjectNum) <= MAX_SUBJECTS)
#define isValidNumOptions(num) ((num) >= 1 && (num) <= MAX_OPTIONS)

typedef void (*MenuFunction)();
typedef struct {
    char name[50];
    MenuFunction func;
} MenuItem;

typedef struct complexQuestion {
    char subject[MAX_OPT_TEXT];
    char question[MAX_OPT_TEXT];
    char answer[MAX_OPTIONS][MAX_OPT_TEXT];
    int correctAnswer;
} CQ;

// Глобальні змінні (extern)
extern int totalInDb;
extern FILE* database;
extern const char* DB_NAME;
extern char subjectNames[MAX_SUBJECTS][MAX_OPT_TEXT];

// Прототипи: FilesController
int checkInputFileType(char* arg);
void readFilePath(int argc, char** argv);
void loadSubjectsFromDB();
CQ* readQuestions();
void inputQuestion();
void calculateNumObjInDB();

// Прототипи: MenuController
int containsIgnoreCase(const char* haystack, const char* needle);
int findAndPrintQuestions(const char* target);
void deleteQuestionByIndex(int indexToDelete);
void printQuestionRow(int num, const CQ* q);
void printTableHeader();

// Прототипи: Menu
void addQuestion();
void searchQuestion();
void deleteQuestion();
void groupQuestionsBySubject();
void printDatabase();
void testing();
void totalTesting();
void customTesting();
void exitProgram();

// Прототипи: TestController
void shuffle(int* array, int n);
int askQuestion(CQ* q, int current, int total);
int runSubjectLogic(CQ* allData, const char* subjectName, int* selectedCount);
int runSubjectsTest(CQ* allData, int selectCount);
int runCustomTestLogic(CQ* allData, int* indices, int count, int toSelect);

#endif