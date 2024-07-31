#include <kernel/types.h>
#include <user/user.h>

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
    int byte = 0;
    write(fds[1], &byte, sizeof(byte));

    char buf[64];
    memset(buf, 0, sizeof(buf));
    if (read(fds[0], buf, sizeof(buf))) {
      printf("received: ping\n");
    }
  } else {
    int byte = 0;
    write(fds[0], &byte, sizeof(byte));

    char buf[64];
    memset(buf, 0, sizeof(buf));
    sleep(10);
    if (read(fds[1], buf, sizeof(buf))) {
      printf("received: pong\n");
    }

    int st = 0;
    wait(&st);
    if (st < 0) {
      fprintf(2, "pingpong: child programs exec failed.\n");
      exit(-2);
    }
  }

  close(fds[0]);
  close(fds[1]);

  exit(0);
}
