#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { false = 0, true } bool;

typedef struct {
	unsigned long ISBN;
	char title[300];
	char author[150];
}EBook, * p_EBook;

typedef struct {
	bool isISBN, isTitle, isAuthor;
}isInserted;

typedef struct {
	bool isLong;
	char* shortened_str;
}strlenTestRes;

void num_of_Ebooks(int* num);
void ISBN_insert(int num, p_EBook arr);
void title_insert(int num, p_EBook arr);
void author_insert(int num, p_EBook arr);

p_EBook ISBN_sort(int num, p_EBook arr, p_EBook m_arr);
p_EBook title_sort(int num, p_EBook arr, p_EBook m_arr);
p_EBook author_sort(int num, p_EBook arr, p_EBook m_arr);

void write_file(const char* p, int num, p_EBook arr);

p_EBook fetch_arr(int num, p_EBook arr, p_EBook m_arr);
void swap(p_EBook book1, p_EBook book2);
void print(const char* p, int num, p_EBook arr); // , bool isISBN, bool isTitle, bool isAuthor);
strlenTestRes isTitleLong(const int len, char* title);
strlenTestRes isAuthorLong(const int len, char* author);

int main(void) {
	int num = 0;
	num_of_Ebooks(&num);
	printf("number of EBooks: %d\n==============\n", num);

isInserted Bookshelf = { 0,0,0 };
	p_EBook arr = (p_EBook)malloc(num * sizeof(EBook));
	ISBN_insert(num, arr);
	title_insert(num, arr);
	author_insert(num, arr);

	
	p_EBook m_arr = (p_EBook)malloc(num * sizeof(EBook));

	m_arr = ISBN_sort(num, arr, m_arr);
	print("Sorted by ISBN", num, m_arr);
	write_file("Sorted by ISBN", num, m_arr);

	m_arr = title_sort(num, arr, m_arr);
	print("Sorted by title", num, m_arr);
	write_file("Sorted by title", num, m_arr);

	m_arr = author_sort(num, arr, m_arr);
	print("Sorted by author", num, m_arr);
	write_file("Sorted by author", num, m_arr);

	return 0;
}

void num_of_Ebooks(int* num) {
	FILE* ISBN = fopen("ISBN.txt", "r");
	while (!feof(ISBN)) {
		fscanf(ISBN, "%d\n");
		*num += 1;
	}
	fclose(ISBN);
	return;
}

void ISBN_insert(int num, p_EBook arr) {
	FILE* ISBN = fopen("ISBN.txt", "r");

	if (ISBN == NULL) {	// 파일을 열기를 성공/실패 했을 경우
		printf("파일 열기 실패\n");
		exit(1);	// 강제 종료
	}
	else
		printf("파일 열기 성공\n");

	int i = 0;
	while (!feof(ISBN)) {
		fscanf(ISBN, "%lu\n", &arr[i].ISBN);
		i++;
	}
	fclose(ISBN);

	printf("\n\n==========ISBN inserted==========\n");
	
	return;
}

void title_insert(int num, p_EBook arr) {
	FILE* title = fopen("TITLE.txt", "r");

	if (title == NULL) {	// 파일을 열기를 성공/실패 했을 경우
		printf("파일 열기 실패\n");
		exit(1);	// 강제 종료
	}
	else
		printf("파일 열기 성공\n\n\n");

	int i = 0;
	char s_char;
	while (!feof(title)) {
		int s = 0;
		for (s; s < 300; s++) {
			if ((s_char = fgetc(title)) != '\n') {
				if (s_char >= 'A' && s_char <= 'Z')
					s_char = s_char - ('A' - 'a');
				arr[i].title[s] = s_char;
			}
			else {
				break;
			}
		}
		arr[i].title[s] = '\0';
		if (i + 1 == num) break;
		i++;
	}
	fclose(title);

	printf("\n\n==========TITLE inserted==========\n");

	return;
}

void author_insert(int num, p_EBook arr) {
	FILE* author = fopen("AUTHOR.txt", "r");

	if (author == NULL) {	// 파일을 열기를 성공/실패 했을 경우
		printf("파일 열기 실패\n");
		exit(1);	// 강제 종료
	}
	else
		printf("파일 열기 성공\n\n\n");

	int i = 0;
	char s_char;
	while (!feof(author)) {
		int s = 0;
		for (s; s < 150; s++) {
			if ((s_char = fgetc(author)) != '\n') {
				if (s_char >= 'A' && s_char <= 'Z')
					s_char = s_char - ('A' - 'a');
				arr[i].author[s] = s_char;
			}
			else {
				break;
			}
		}
		arr[i].author[s] = '\0';
		if (i + 1 == num) break;
		i++;
	}
	fclose(author);

	printf("\n\n==========AUTHOR inserted==========\n");

	return;
}

p_EBook ISBN_sort(int num, p_EBook arr, p_EBook m_arr) {
	printf("ISBN sorting...\n");
	m_arr = fetch_arr(num, arr, m_arr);

	for (int i = 0; i < num; i++) {
		for (int j = 0; j < num - i - 1; j++) {
			if (m_arr[j].ISBN > m_arr[j + 1].ISBN) {
				swap(&m_arr[j + 1], &m_arr[j]);
			}
		}
		// for Debugging
		if (i%1000==0) printf("[Sorting by ISBN] NOW:...%7d\n", i);
	}

	return m_arr;
}

p_EBook title_sort(int num, p_EBook arr, p_EBook m_arr) {
	printf("Title sorting...\n");
	fetch_arr(num, arr, m_arr);

	for (int i = 0; i < num; i++) {
		for (int j = 0; j < num - i - 1; j++) {
			if (strcmp(m_arr[j].title, m_arr[j + 1].title) > 0) {
				swap(&m_arr[j + 1], &m_arr[j]);
			}
		}
	}

	return m_arr;
}

p_EBook author_sort(int num, p_EBook arr, p_EBook m_arr) {
	printf("Author sorting...\n");
	fetch_arr(num, arr, m_arr);

	for (int i = 0; i < num; i++) {
		for (int j = 0; j < num - i - 1; j++) {
			if (strcmp(m_arr[j].author, m_arr[j + 1].author) > 0) {
				swap(&m_arr[j + 1], &m_arr[j]);
			}
		}
	}

	return m_arr;
}

void write_file(const char* p, int num, p_EBook arr) {
	char* purp = (char*)malloc((strlen(p) + 10) * sizeof(char));
	purp = "Noob";
	purp = strdup(p);
	strcat(purp, ".txt");
	printf("opening %s to write...\n", purp);
	FILE* newFile = fopen(purp, "w");

	for (int i = 0; i < num; i++) {
		if (isTitleLong(80, arr[i].title).isLong == 1) {
			strcpy(arr[i].title, isTitleLong(80, arr[i].title).shortened_str);
		}

		if (isAuthorLong(80, arr[i].author).isLong == 1) {
			strcpy(arr[i].author, isTitleLong(80, arr[i].author).shortened_str);
		}
		//printf("now...[%-5d]\n", i + 1);
		fprintf(newFile, "[%-5d] ", i + 1);
		fprintf(newFile, "ISBN: %-10lu TITLE: %-80s... AUTHER: %-80s...\n", arr[i].ISBN, arr[i].title, arr[i].author);
	}
	fclose(newFile);
	return;
}


p_EBook fetch_arr(int num, p_EBook arr, p_EBook m_arr) {
	for (int i = 0; i < num; i++) {
		m_arr[i] = arr[i];
	}
	return m_arr;
}

void swap(p_EBook book1, p_EBook book2) {	// 위치 바꾸는 함수
	EBook temp;

	temp = *book2;
	*book2 = *book1;
	*book1 = temp;
}

void print(const char* pps, int num, p_EBook arr) {  //, bool isISBN, bool isTitle, bool isAuthor) {
	printf("\n\n============<<%s>>=============\n", pps);
	for (int i = 0; i < num; i++) {
		if (isTitleLong(40, arr[i].title).isLong == 1) {
			strcpy(arr[i].title, isTitleLong(40, arr[i].title).shortened_str);
		}

		if (isAuthorLong(40, arr[i].author).isLong == 1) {
			strcpy(arr[i].author, isTitleLong(30, arr[i].author).shortened_str);
		}

		printf("[%-5d] ", i + 1);
		printf("ISBN: %-10lu TITLE: %-40s... AUTHER: %-30s...\n", arr[i].ISBN, arr[i].title, arr[i].author);
	}
	return;
}

strlenTestRes isTitleLong(const int len, char* title) {
	bool isLong = 0;
	char* abb_title = (char*)malloc((len + 1) * sizeof(char));

	if (strlen(title) > len+1) {
		isLong = 1;
		int o;
		for (o = 0; o < len; o++) {
			abb_title[o] = title[o];
		}
		*(abb_title + o) = '\0';
	}

	strlenTestRes res = { isLong, abb_title };
	return res;
}

strlenTestRes isAuthorLong(const int len, char* author) {
	bool isLong = 0;
	char* abb_author = (char*)malloc((len + 1) * sizeof(char));

	if (strlen(author) > len) {
		isLong = 1;
		int o;
		for (o = 0; o < len; o++) {
			abb_author[o] = author[o];
		}
		*(abb_author+o) = '\0';
	}

	strlenTestRes res = { isLong, abb_author };
	return res;
}

