#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	char buffer[12];

	strcpy(buffer, argv[1]);

	printf("%s\n", buffer);

	return 0;
}
