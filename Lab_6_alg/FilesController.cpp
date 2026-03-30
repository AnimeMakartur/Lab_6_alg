#include "Header.h"

// Визначення глобальних змінних
int totalInDb = 0;
FILE* database = NULL;
const char* DB_NAME = "D:\\Files\\database.bin";
char subjectNames[MAX_SUBJECTS][MAX_OPT_TEXT];

int checkInputFileType(char* arg) {
	char* lastDot = strrchr(arg, '.');
	return (lastDot != NULL && _stricmp(lastDot + 1, "bin") == 0);
}
void readFilePath(int argc, char** argv) {
	if (argc > 1 && checkInputFileType(argv[1])) {
		if (strchr(argv[1], ':') || strchr(argv[1], '\\')) {
			printf("Path detected: %s\n", argv[1]);
		}
		fopen_s(&database, argv[1], "rb+");
	}
	else {
		fopen_s(&database, DB_NAME, "rb+");
	}

	if (database == NULL) {
		printf("Initialising database...\n");
		fopen_s(&database, (argc > 1) ? argv[1] : DB_NAME, "wb+");
		inputQuestion();
	}
	else {
		loadSubjectsFromDB();
	}
}
void loadSubjectsFromDB() {
	rewind(database);
	CQ temp;
	int count = 0;

	// Очищаємо поточні назви
	for (int i = 0; i < MAX_SUBJECTS; i++) subjectNames[i][0] = '\0';

	while (fread(&temp, sizeof(CQ), 1, database) && count < MAX_SUBJECTS) {
		int found = 0;
		for (int i = 0; i < count; i++) {
			if (strcmp(subjectNames[i], temp.subject) == 0) {
				found = 1;
				break;
			}
		}
		if (!found) {
			strcpy_s(subjectNames[count++], MAX_OPT_TEXT, temp.subject);
		}
	}
	// Оновлюємо totalInDb про всяк випадок
	fseek(database, 0, SEEK_END);
	calculateNumObjInDB();
}
void calculateNumObjInDB() {
	totalInDb = ftell(database) / sizeof(CQ);
}
void inputQuestion() {
	CQ newQuestion;
	int numSubjects = 0;
	int numQuestionsSubjectOne = 0;
	int numQuestionsSubjectTwo = 0;
	int numQuestionsSubjectThree = 0;
	printf("Enter the list of subjects, max %d:\n", 3);
	for (int i = 0; i < 3; i++)
	{
		gets_s(subjectNames[i], MAX_OPT_TEXT);
	}
	for (int i = 0;; i++)
	{
		char answer[MAX_OPT_TEXT];
		printf("Enter subjects for question: %s - 1, %s - 2, %s - 3: ", subjectNames[0], subjectNames[1], subjectNames[2]);
	inputSubjectLabel:
		scanf_s("%d", &numSubjects);
		rewind(stdin);
		if (!isValidSubjectNum(numSubjects)) {
			printf("Invalid input, try again:");
			goto inputSubjectLabel;
		}
		if (numSubjects == 1) {
			numQuestionsSubjectOne++;
		}
		else if (numSubjects == 2) {
			numQuestionsSubjectTwo++;
		}
		else if (numSubjects == 3) {
			numQuestionsSubjectThree++;
		}
		strcpy_s(newQuestion.subject, MAX_OPT_TEXT, subjectNames[numSubjects - 1]);
		printf("Enter the question text: ");
		gets_s(newQuestion.question, MAX_OPT_TEXT);
		for (int j = 0; j < MAX_OPTIONS; j++)
		{
			printf("Enter answer option %d: ", j + 1);
			gets_s(answer, MAX_OPT_TEXT);
			strcpy_s(newQuestion.answer[j], MAX_OPT_TEXT, answer);
		}
		printf("Enter the number of the correct answer (1-%d): ", MAX_OPTIONS);
	inputRightAnswerLAbel:
		scanf_s("%d", &newQuestion.correctAnswer);
		rewind(stdin);
		if (!isValidNumOptions(newQuestion.correctAnswer)) {
			printf("Invalid input, try again:");
			goto inputRightAnswerLAbel;
		}
		fwrite(&newQuestion, sizeof(CQ), 1, database);
		totalInDb++;
		printf("Question added successfully!\n");
		printf("Current question count for %s: %d\n", subjectNames[0], numQuestionsSubjectOne);
		printf("Current question count for %s: %d\n", subjectNames[1], numQuestionsSubjectTwo);
		printf("Current question count for %s: %d\n", subjectNames[2], numQuestionsSubjectThree);
		if (numQuestionsSubjectOne == numQuestionsSubjectTwo && numQuestionsSubjectTwo == numQuestionsSubjectThree) {
			printf("All subjects have the same number of questions. Do you want continue to input? no - N, yes - Y\n");
		endInput:
			char choice = getchar();
			if (choice == 'n' || choice == 'N') {
				break;
			}
			else if (choice == 'y' || choice == 'Y') {
				continue;
			}
			else {
				printf("Invalid choice. Write N or Y.\n");
				goto endInput;
			}
		}
	}
	printf("Input ends\n");
	return;
}
CQ* readQuestions() {
	rewind(database);
	if (totalInDb == 0) return NULL;
	CQ* allQuestions = (CQ*)malloc(sizeof(CQ) * (totalInDb));
	if (allQuestions == NULL) {
		printf("No memory");
		return NULL;
	}
	fread(allQuestions, sizeof(CQ), totalInDb, database);
	return allQuestions;
}


