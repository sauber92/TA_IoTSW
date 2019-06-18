#include <stdio.h>
#include <string.h>

void ThirdParty_Library() {
	//Codes here
	
	system("/bin/sh");

	//Codes here
}

int main(int argc, char** argv) {
	char buffer[12];

	strcpy(buffer, argv[1]);
	
	printf("%s\n", buffer);

	return 0;
}
