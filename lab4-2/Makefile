app:
	arm-linux-gnueabihf-gcc -c newsyscall.c
	ar ruv newsyscall.a newsyscall.o
	arm-linux-gnueabihf-gcc -o syscall_app syscall_app.c newsyscall.a
