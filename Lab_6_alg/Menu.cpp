#include "Header.h"
#include <ctype.h>

void groupQuestionsBySubject() {
	if (totalInDb <= 0) return;

	CQ* allData = readQuestions();
	if (allData == NULL) return;

	// Сортування (Bubble Sort)
	for (int i = 0; i < totalInDb - 1; i++) {
		for (int j = 0; j < totalInDb - i - 1; j++) {
			if (strcmp(allData[j].subject, allData[j + 1].subject) > 0) {
				CQ temp = allData[j];
				allData[j] = allData[j + 1];
				allData[j + 1] = temp;
			}
		}
	}

	// ВАЖЛИВО: Перевідкриваємо файл у режимі "wb", щоб стерти старі дані
	fclose(database);
	fopen_s(&database, DB_NAME, "wb+");

	if (database != NULL) {
		fwrite(allData, sizeof(CQ), totalInDb, database);

		// Повертаємо режим до "rb+" для подальшої роботи
		fclose(database);
		fopen_s(&database, DB_NAME, "rb+");
	}

	printf("Questions grouped and database updated.\n");
	free(allData);
}
void addQuestion() {
	CQ addedQuestion;
	if (database == NULL) return;
	int numSubjects = 0;
	puts("Adding question to DataBase:");
	int i = 0;
	printf("Enter subjects for question: %s - 1, %s - 2, %s - 3: ", subjectNames[0], subjectNames[1], subjectNames[2]);
inputSubjectLabel:
	scanf_s("%d", &numSubjects);
	rewind(stdin);
	if (!isValidSubjectNum(numSubjects)) {
		printf("Invalid input, try again:");
		goto inputSubjectLabel;
	}
	strcpy_s(addedQuestion.subject, MAX_OPT_TEXT, subjectNames[numSubjects - 1]);
	printf("Question: ");
	gets_s(addedQuestion.question, MAX_OPT_TEXT);
	for (int i = 0; i < MAX_OPTIONS; i++) {
		printf("Answer %d: ", i + 1);
		gets_s(addedQuestion.answer[i], MAX_OPT_TEXT);
	}
	printf("Enter the number of the correct answer (1-%d): ", MAX_OPTIONS);
inputRightAnswerLAbel:
	scanf_s("%d", &addedQuestion.correctAnswer);
	rewind(stdin);
	if (!isValidNumOptions(addedQuestion.correctAnswer)) {
		printf("Invalid input, try again:");
		goto inputRightAnswerLAbel;
	}
	totalInDb++;
	fwrite(&addedQuestion, sizeof(CQ), 1, database);
	i++;
	printf("Questions Adedd\n");
}
void searchQuestion() {
	char target[MAX_OPT_TEXT];

	puts("--- Universal Search (Case Insensitive) ---");

	while (1) {
		printf("Enter text to search: ");
		rewind(stdin);
		if (gets_s(target, MAX_OPT_TEXT) != NULL && strlen(target) > 0) {
			// Перевірка, чи рядок не складається лише з пробілів
			int onlySpaces = 1;
			for (int i = 0; i < (int)strlen(target); i++) {
				if (!isspace((unsigned char)target[i])) {
					onlySpaces = 0;
					break;
				}
			}
			if (!onlySpaces) break;
		}
		printf("Invalid input. Please enter at least one character.\n");
	}

	findAndPrintQuestions(target); 
}
void printDatabase() {
	CQ q;
	int i = 1;
	if (database == NULL) {
		printf("Database is empty or doesn't exist\n");
		return;
	}

	rewind(database);
	printTableHeader();

	while (fread(&q, sizeof(CQ), 1, database)) {
		printQuestionRow(i++, &q);
	}
	printf("--------------------------------------------------------------------------------------------------------------------------------------------\n");
}
void deleteQuestion() {
	if (totalInDb == 0) {
		printf("Database is empty!\n");
		return;
	}

	char target[MAX_OPT_TEXT];
	printf("--- Selective Deletion (Case Insensitive) ---\n");
	while (1) {
		printf("Enter search term: ");
		rewind(stdin); // Очищення перед gets_s
		if (gets_s(target, MAX_OPT_TEXT) != NULL && strlen(target) > 0) {
			int onlySpaces = 1;
			for (int i = 0; i < (int)strlen(target); i++) {
				if (!isspace((unsigned char)target[i])) {
					onlySpaces = 0;
					break;
				}
			}
			if (!onlySpaces) break;
		}
		printf("Invalid input.\n");
	}

	printf("\nSearch results for deletion:\n");
	int foundCount = findAndPrintQuestions(target);
	if (foundCount == 0) {
		printf("End function\n");
		return;
	}
	int choice;
	printf("\nEnter the 'Num' to delete (0 to cancel): ");

	// Перевірка введення числа
	if (scanf_s("%d", &choice) != 1) {
		printf("Invalid input. Numbers only!\n");
		rewind(stdin); // Очищуємо "сміття", якщо ввели літери
		return;
	}

	// Якщо ввели 0 або число поза діапазоном
	if (choice == 0) {
		printf("Operation cancelled by user.\n");
		rewind(stdin); // Важливо очистити буфер після scanf
		return;
	}

	if (choice < 1 || choice > totalInDb) {
		printf("Invalid number. Out of range.\n");
		rewind(stdin);
		return;
	}

	// Підтвердження
	printf("Are you sure you want to delete question #%d? (y/n): ", choice);
	rewind(stdin); // Очищення перед getchar()
	char confirm = getchar();

	if (confirm == 'y' || confirm == 'Y') {
		deleteQuestionByIndex(choice);
	}
	else {
		printf("Deletion aborted.\n");
	}

	rewind(stdin); // Фінальне очищення для наступних пунктів меню
}
void testing() {
	CQ* allData = readQuestions();
	if (allData == NULL || totalInDb == 0) {
		printf("Database is empty!\n");
		return;
	}
	int correctPerSub[MAX_SUBJECTS] = { 0, 0, 0 };
	int selectedPerSub[MAX_SUBJECTS] = { 0, 0, 0 };
	int totalCorrect = 0;
	int totalSelected = 0;
	system("cls || clear");
	printf("========== STARTING TEST ==========\n");

	for (int i = 0; i < MAX_SUBJECTS; i++) {
		if (strlen(subjectNames[i]) > 0) {
			// Running test logic for each subject
			correctPerSub[i] = runSubjectLogic(allData, subjectNames[i], &selectedPerSub[i]);
			totalCorrect += correctPerSub[i];
			totalSelected += selectedPerSub[i];
		}
	}
	printf("\n================ TEST RESULTS =================\n");
	printf("%-20s | %-10s | %-10s\n", "Subject", "Correct", "Percentage");
	printf("---------------------------------------------\n");

	for (int i = 0; i < MAX_SUBJECTS; i++) {
		if (selectedPerSub[i] > 0) {
			double percent = ((double)correctPerSub[i] / selectedPerSub[i]) * 100.0;
			printf("%-20.20s | %d/%-8d | %.1f%%\n",
				subjectNames[i], correctPerSub[i], selectedPerSub[i], percent);
		}
	}

	printf("---------------------------------------------\n");
	if (totalSelected > 0) {
		printf("OVERALL RESULT: %.1f%%\n", ((double)totalCorrect / totalSelected) * 100.0);
	}
	printf("=============================================\n");

	free(allData);
	printf("\nPress Enter to return to menu...");
	rewind(stdin);
	getchar();
}
void totalTesting() {
	CQ* allData = readQuestions();
	int qPerSubject = totalInDb / MAX_SUBJECTS;
	int selectCount = (int)(totalInDb * 0.75);
	if (selectCount == 0 && totalInDb > 0) selectCount = 1;
	system("cls || clear");
	puts("========== Test ==========");
	int correct = runSubjectsTest(allData, selectCount);
	if (correct == -1) {
		printf("Error in test, try again\n");
		return;
	}
	printf("\n============================================\n");
	printf("TEST FINISHED!\n");
	printf("Total questions: %d\n", selectCount);
	printf("Correct answers: %d\n", correct);
	printf("Your score: %.1f%%\n", ((double)correct / selectCount) * 100.0);
	printf("============================================\n");
	free(allData);
	printf("\nPress Enter to return...");
	while (getchar() != '\n'); // Очистка буфера
	getchar();

}
void customTesting() {
	if (totalInDb == 0) {
		printf("Database is empty!\n");
		return;
	}

	CQ* allData = readQuestions();
	if (!allData) return;

	int mode;
	printf("\n--- Custom Test Setup ---\n");
	printf("1. All subjects\n");
	printf("2. Specific subject\n");
	do {
		printf("Choice: ");
		scanf_s("%d", &mode);
		rewind(stdin);
		if (mode < 1 || mode > 2 ) {
			printf("Invalid input must be 1 or 2.\n");
		}
	} while (mode < 1 || mode > 2);


	int* targetIndices = (int*)malloc(sizeof(int) * totalInDb);
	if (targetIndices == NULL) {
		printf("No enough memory\n");
		free(allData);
		return;
	}
	int count = 0;
	switch (mode) {
	case 1:
		for (int i = 0; i < totalInDb; i++) {
			targetIndices[count++] = i;
		}
		break;
	case 2 :
	label:
		printf("Select subject: %s - 1, %s - 2, %s - 3: ", subjectNames[0], subjectNames[1], subjectNames[2]);
		int subIdx;
		scanf_s("%d", &subIdx);
		rewind(stdin);

		if (subIdx < 1 || subIdx > 3) {
			printf("Invalid subject choice.\n");
			goto label;
		}
		else {
			for (int i = 0; i < totalInDb; i++) {
				if (strcmp(allData[i].subject, subjectNames[subIdx - 1]) == 0) {
					targetIndices[count++] = i;
				}
			}
		}
		break;
	default: 
		printf("Invalid Input");
		break;
	}

	if (count == 0) {
		printf("No questions found for your criteria.\n");
		free(targetIndices);
		free(allData);
		return;
	}

	int requestedCount;
	do {
		printf("Found %d questions. How many do you want to answer? ", count);
		scanf_s("%d", &requestedCount);
		rewind(stdin);
		if (requestedCount <1 || requestedCount > count) {
			printf("Invalid input, number must in 1 and %d\n", count);
		}
	} while (requestedCount < 1 || requestedCount > count);
	
	system("cls || clear");
	int score = runCustomTestLogic(allData, targetIndices, count, requestedCount);

	printf("\n==============================");
	printf("\nTest Finished!");
	printf("\nScore: %d/%d (%.1f%%)", score, requestedCount, (score * 100.0) / requestedCount);
	printf("\n==============================\n");

	free(targetIndices);
	free(allData);
}
void exitProgram() {
	if (database) fclose(database);
	exit(0);
}
