#include <iostream>
#include <io.h>
#include <Windows.h>
#include <string.h>
#include <ctype.h>
#include <sys/locking.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

/*
struct Node {
	int x;
	Node* l, * r;
};
void show(Node*& Tree) {
	if (Tree != nullptr) {
		show(Tree->l);
		cout << Tree->x << " ";
		show(Tree->r);
	}
}
void addNode(int x, Node*& Tree) {
	if (Tree == nullptr) {
		Tree = new Node;
		Tree->x = x;
		Tree->l = Tree->r = nullptr;
	}
	if (x < Tree->x) {
		if (Tree->l != nullptr) addNode(x, Tree->l);
		else {
			Tree->l = new Node;
			Tree->l->l = Tree->l->r = nullptr;
			Tree->l->x = x;
		}
	}
	if (x > Tree->x) {
		if (Tree->r != nullptr) addNode(x, Tree->r);
		else {
			Tree->r = new Node;
			Tree->r->l = Tree->r->r = nullptr;
			Tree->r->x = x;
		}
	}
}
void del(Node*& Tree) {
	if (Tree != nullptr) {
		del(Tree->l);
		del(Tree->r);
		delete Tree;
		Tree = nullptr;
	}
}
*/
/*int main()
{
	Node* Tree = nullptr;
	for (int i = 5; i > 0; i--) {
		addNode(i, Tree);
	}
	show(Tree);
	del(Tree);
	cout << endl;
	for (int i = 20; i > 5; i--) {
		addNode(i, Tree);
	}
	show(Tree);
	del(Tree);
}*/

/*
//stdio.h
	// FILE *fopen_s(const char* filename, const char* mode);
		// r    r+
		// w    w+
		// a    a+
	// int fclose(FILE* stream); EOF
	// char* fgets(char* str, int n, FILE* stream);
	// int fputs(const char* str, FILE* stream); EOF
	// size_t fread(void* buffer, size_t size, size_t count, FILE* strream);
	// size_t fwrite(void* buffer, size_t size, size_t count, FILE* strream);
	// int feof(FILE* stream);
	// int _fileno (FILE* stream);
	// int fseek(FILE* stream, int offset [, int whence]);
		// SEEK_SET (0)
		// SEEK_CUR (1)
		// SEEK_END (2)
// io.h
	// int _access(const char* path, int mode);
		// 00
		// 02
		// 04
		// 06
	// _filelenght(int handle);
	// int _locking(int handle, int mode, long nbytes);
		// _LK_LOCK
		// _LK_NBLCK
		// _LK_UNLCK


//Копирование файла. Пользователь с клавиатуры вводит путь к файлу и путь для копии

bool CopyFile(const char* source, const char* destination) {
	const int size = 65536;
	FILE* src, * dest;
	if (fopen_s(&src, source, "rb")) return false;

	size_t handle = _fileno(src);

	char* data = new char[size];
	if (!data) return false;

	if (fopen_s(&dest, destination, "wb")) {
		delete[]data;
		return false;
	}

	size_t realsize;
	while (!feof(src)) {
		realsize = fread(data, sizeof(char), size, src);
		fwrite(data, sizeof(char), realsize, dest);
	}
	fclose(src);
	fclose(dest);

	return true;
}


int main() {
	char source[_MAX_PATH];
	char destination[_MAX_PATH];
	char answer[20];

	setlocale(LC_ALL, "");

	cout << "Введите путь и название копируемого файла: ";
	cin.getline(source, _MAX_PATH);
	if (_access(source, 00) == -1) {
		cout << "Такого файла не существует\n";
		return -1;
	}
	cout << "Введите путь и название нового файла: ";
	cin.getline(destination, _MAX_PATH);
	if (_access(destination, 00) == 0) {
		cout << "Такой файл уже существует. Перезаписать? (1 - Да / 2 - Нет): ";
		cin.getline(answer, 20);
		if (!strcmp(answer, "2")) {
			cout << "Копирование отменено!";
			return -1;
		}
		else if (strcmp(answer, "1")) {
			cout << "Неверный ввод!";
			return -1;
		}
		if (_access(destination, 02) == -1) {
			cout << "Нет доступа к записи!";
			return -1;
		}
	}
	if (!CopyFile(source, destination)) {
		cout << "Произошла ошибка копирования!";
	}
}
*/

#define MAX_WORD_LENGTH 21

int tries = 10;
int countWords = 0;

bool LoadWord(FILE* file, char* word) {
	int i = 0;
	char s[MAX_WORD_LENGTH]{ 0 };
	static int count = -1;

	if (count == -1) {
		while (!feof(file)) {
			fgets(s, MAX_WORD_LENGTH, file);
			count++;
		}
		if (count == 0) return false;
		fseek(file, 0, 0);
	}

	int n = rand() % count;
	while (i <= n) {
		fgets(s, MAX_WORD_LENGTH, file);
		i++;
	}

	int wordLen = strlen(s);
	if (s[wordLen - 1] == 10) s[wordLen - 2] = 0;
	else if (s[wordLen - 1] == 13) s[wordLen - 1] = 0;

	strcpy_s(word, strlen(word) + 1, s);
	size_t handle = _fileno(file);
	int size = _filelength(handle);
	fseek(file, 0, 0);
	_locking(handle, _LK_NBLCK, size);
	return true;
}

void Game(char* word) {
	_strupr_s(word, strlen(word) + 1);

	int len = strlen(word);
	char* copy = new char[len + 1];
	memset(copy, '*', len);
	copy[len] = 0;

	char letters[52];
	for (int i = 0, j = 0; i < 26; i++) {
		letters[j++] = i + 'A';
		letters[j++] = ' ';
	}

	letters[51] = 0;

	char letter;
	char* pos = nullptr;
	bool replace = false;

	do {
		system("cls");
		cout << copy << endl << endl;
		cout << letters << endl << endl;
		cout << "Количество попыток: " << tries << endl << endl;
		cout << "Введите букву -> "; cin >> letter;

		Beep(500, 200);

		if (!isalpha(letter)) {
			cout << "Это не буква!" << endl;
			Sleep(1000);
			continue;
		}
		letter = toupper(letter);
		pos = strchr(letters, letter);
		if (pos == nullptr) {
			cout << "Эта буква уже была использована" << endl;
			Sleep(1000);
			continue;
		}
		else {
			pos[0] = ' ';
		}

		for (int i = 0; i < len; i++) {
			if (word[i] == letter) {
				copy[i] = letter;
				replace = true;
			}
		}
		if (replace == false) tries--;
		else replace = false;

		if (strcmp(word, copy) == 0) {
			system("cls");
			cout << copy << endl << endl;
			cout << letters << endl << endl;
			cout << "Количество попыток: " << tries << endl << endl;
			cout << "ПОБЕДА!!!" << endl;
			countWords++;
			break;
		}
	} while (tries != 0);
	delete[]copy;
}

void main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(0));
	FILE* f;
	if (!fopen_s(&f, "C:\\Text\\text.txt", "rb")) {
		char word[20];
		if (!LoadWord(f, word)) {
			cout << "Ошибка!!!" << endl;
			fclose(f);
			return;
		}

		char answer;

		do {
			Game(word);
			if (tries == 0) {
				cout << "Попыток осталось " << tries << endl << "Пока";
				break;
			}
			cout << "Продолжить игру?(Д / Н) -> ";
			cin >> answer;
			if (answer == 'Д') {
				if (!LoadWord(f, word)) {
					cout << "Ошибка!!!" << endl;
					fclose(f);
					return;
				}
			}
		} while (answer == 'Д');

		size_t handle = _fileno(f);
		int size = _filelength(handle);
		fseek(f, 0, 0);
		_locking(handle, _LK_UNLCK, size);
		fclose(f);
	}
}