#include <unistd.h>

#define nr_syscall_crash 376

int main(void)
{
	syscall(nr_syscall_crash);

	return 0;
}

