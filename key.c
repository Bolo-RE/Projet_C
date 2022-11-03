#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {
	char input = 0;
	do {
		input = getch();
		printf("%d\n", (int)input);
	} while (input != 0);
}
