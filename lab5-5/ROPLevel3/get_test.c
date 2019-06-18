#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main() {
	char input[8];

	while(1) {
		gets(input);
		printf("inputl = %s\n", input);
	}

	return 0;
}
