#include "Header.h"
#include <ctype.h>
#include <string.h>

void printQuestionRow(int num, const CQ* q) {
	printf("%-3d | %-15.15s | %-20.20s | %-15.15s | %-15.15s | %-15.15s | %-15.15s | %-5d\n",
		num,
		q->subject,
		q->question,
		q->answer[0],
		q->answer[1],
		q->answer[2],
		q->answer[3],
		q->correctAnswer);
}
void printTableHeader() {
	printf("\n%-3s | %-15s | %-20s | %-15s | %-15s | %-15s | %-15s | %-5s\n",
		"Num", "Subject", "Question", "Ans 1", "Ans 2", "Ans 3", "Ans 4", "Right");
	printf("--------------------------------------------------------------------------------------------------------------------------------------------\n");
}
int containsIgnoreCase(const char* haystack, const char* needle) {
	// Перевірка на NULL (захист від некоректних даних)
	if (haystack==NULL || needle==NULL) return 0;

	// Порожній рядок вважається підрядком будь-якого рядка
	if (*needle == '\0') return 1;

	char h[MAX_OPT_TEXT], n[MAX_OPT_TEXT];
	char* pDest, * pSrc;

	// Копіюємо haystack в h, перетворюючи в нижній регістр за допомогою вказівників
	pDest = h;
	pSrc = (char*)haystack;
	while (*pSrc) {
		*pDest++ = tolower(*pSrc++);
	}
	*pDest = '\0';

	// Копіюємо needle в n, перетворюючи в нижній регістр за допомогою вказівників
	pDest = n;
	pSrc = (char*)needle;
	while (*pSrc) {
		*pDest++ = tolower(*pSrc++);
	}
	*pDest = '\0';

	// strstr повертає вказівник на початок знайденого підрядка або NULL
	return (strstr(h, n) != NULL);
}
int findAndPrintQuestions(const char* target) {
	if (target == NULL || strlen(target) == 0 || target[0] == '\n') {
		printf("Search term cannot be empty!\n");
		return 0;
	}
	CQ* allData = readQuestions();
	CQ* pAllData = allData;
	int foundCount = 0;
	int currentIdx = 1;
	int match = 0;
	if (database == NULL) return 0;
	rewind(database);
	printTableHeader(); // Використовуємо спільний заголовок
	for (CQ* pTotal = allData+totalInDb; pAllData < pTotal; pAllData++) {
		if (containsIgnoreCase(pAllData->subject, target)) match = 1;
		else if (containsIgnoreCase(pAllData->question, target)) match = 1;
		else {
			for (int j = 0; j < MAX_OPTIONS; j++) {
				if (containsIgnoreCase(pAllData->answer[j], target)) {
					match = 1;
					break;
				}
			}
		}
		if (match) {
			printQuestionRow(currentIdx, pAllData); // Вивід з усіма відповідями
			foundCount++;
		}
		currentIdx++;
	}
	if (foundCount == 0) {
		printf("No matches found for: '%s'\n", target);
	}
	else {
		printf("--------------------------------------------------------------------------------------------------------------------------------------------\n");
		printf("Total matches: %d\n", foundCount);
	}
	free(allData);
	return foundCount;
}
void deleteQuestionByIndex(int indexToDelete) {
	CQ* allData = readQuestions(); // Зчитуємо всі дані в пам'ять [cite: 7, 14]
	if (!allData) return;

	// Перевідкриваємо файл для повного перезапису 
	fclose(database);
	fopen_s(&database, DB_NAME, "wb+");

	for (int i = 0; i < totalInDb; i++) {
		// Записуємо всі елементи, крім того, що під обраним номером
		if (i != (indexToDelete - 1)) {
			fwrite(&allData[i], sizeof(CQ), 1, database);
		}
	}
	totalInDb--;
	free(allData);
	fclose(database);
	fopen_s(&database, DB_NAME, "rb+");
	printf("Successfully deleted question #%d\n", indexToDelete);
}