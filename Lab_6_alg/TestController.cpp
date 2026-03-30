#include "Header.h"
#include <time.h>

void shuffle(int* array, int n) {
	for (int i = n - 1; i > 0; i--) {
		int j = rand() % (i + 1);
		int temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}
}

int askQuestion(CQ* q, int current, int total) {
	int userAns;
	printf("\n[%d/%d] %s\n", current, total, q->question);
	for (int i = 0; i < MAX_OPTIONS; i++) printf("%d. %s\n", i + 1, q->answer[i]);

	while (scanf_s("%d", &userAns) != 1 || !isValidNumOptions(userAns)) {
		printf("Invalid! Try again: ");
		rewind(stdin);
	}
	return (userAns == q->correctAnswer);
}
int runSubjectLogic(CQ* allData, const char* subjectName, int* selectedCount) {
	int subjectIndices[MAX_QUESTIONS];
	int countInDb = 0;
	int correctAnswers = 0;
	for (int i = 0; i < totalInDb; i++) {
		if (strcmp(allData[i].subject, subjectName) == 0) {
			subjectIndices[countInDb++] = i;
		}
	}
	if (countInDb == 0) {
		*selectedCount = 0;
		return 0;
	}
	int toSelect = (int)(countInDb * 0.75);
	if (toSelect == 0) toSelect = 1;
	*selectedCount = toSelect;
	shuffle(subjectIndices, countInDb);
	for (int i = 0; i < toSelect; i++) {
		correctAnswers += askQuestion(&allData[subjectIndices[i]], i + 1, toSelect);
	}
	return correctAnswers;
}
int runSubjectsTest(CQ* allData, int selectCount) {
	int* indices = (int*)malloc(sizeof(int) * totalInDb);
	if (indices == NULL) {
		printf("No enough memory");
		return -1;
	}
	for (int i = 0; i < totalInDb; i++) {
		indices[i] = i;
	}
	shuffle(indices, totalInDb);
	int correct = 0;
	system("cls || clear");
	printf("========== START TEST (%d questions) ==========\n", selectCount);
	int idx = 0;
	for (int i = 0; i < selectCount; i++) {
		CQ* q = &allData[indices[i]];
		printf("\n[%d/%d] Question: %s\n", i + 1, selectCount, q->question);
		for (int opt = 0; opt < MAX_OPTIONS; opt++) {
			printf("  %d. %s\n", opt + 1, q->answer[opt]);
		}
		int userAns;
	inputRightAnswerLabel1:
		printf("Your answer: ");
		scanf_s("%d", &userAns);
		if (!isValidNumOptions(userAns)) {
			printf("Invalid input, try again:");
			goto inputRightAnswerLabel1;
		}

		if (userAns == q->correctAnswer) {
			correct++;
		}
		printf("--------------------------------------------\n");
	}
	free(indices);
	return correct;
}

int runCustomTestLogic(CQ* allData, int* indices, int count, int toSelect) {
	if (toSelect > count) toSelect = count;

	shuffle(indices, count);
	int correct = 0;

	printf("\n--- Starting Custom Test (%d questions) ---\n", toSelect);
	for (int i = 0; i < toSelect; i++) {
		if (askQuestion(&allData[indices[i]], i + 1, toSelect)) {
			correct++;
		}
	}
	return correct;
}