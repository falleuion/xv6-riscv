#include <kernel/types.h>
#include <user/user.h>

static int primeTable[10] = { 2, 3, 7, 11, 13, 17, 19, 23, 29, 31 };

int
main(int argc, char **argv)
{
  int fds[2];
  int ret = pipe(fds);
  if (ret != 0) {
    fprintf(2, "failed to create pipe\n");
    exit(-1);
  }

  int pid = fork();
  if (pid == 0) {
    int tmp = 0;
    for (int i = 0; i < 10; i++) {
      tmp = primeTable[i];
      if (write(fds[1], &tmp, sizeof(int)) < 0) {
        fprintf(2, "write pipe failed.\n");
      }
    }

    tmp = -1;
    write(fds[1], &tmp, sizeof(int));
  } else {
    int num = 0;
    while (read(fds[0], &num, sizeof(int)) > 0) {
      if (num < 0) {
        break;
      }

      printf("prime %d\n", num);
    }
  }

  close(fds[0]);
  close(fds[1]);

  exit(0);
}
