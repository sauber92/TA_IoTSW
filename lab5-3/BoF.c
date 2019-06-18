#include <stdio.h>
#include <string.h>

void copy_print(char* arg) {
	char buffer[64];

	strcpy(buffer, arg);

	printf("%s\n", buffer);
}

int main(int argc, char** argv) {
	copy_print(argv[1]);

	return 0;
}
