#include "/home/ubuntu/workspace/linux/arch/arm/include/uapi/asm/unistd.h"

int mysyscall(int n, int m) {
  return syscall(__NR_mysyscall, n, m);
}
