#include "kernel/types.h"
#include "kernel/param.h"
#include <user/user.h>

char **
conlist(int argc, char **argv, char *last)
{
  static char *buff[MAXARG];
  memset(buff, 0, sizeof(buff));

  int i = 0;
  for (; i < argc - 1; i++) {
    buff[i] = argv[i];
  }
  buff[i] = last;

  return buff;
}

int
main(int argc, char **argv)
{
  if (argc < 2) {
    fprintf(2, "xargs: too few arguments.\n");
    exit(-1);
  }

  char buf[1024];
  memset(buf, 0, sizeof(buf));
  char *prog = argv[1];

  gets(buf, 1024);
  char **arglist = conlist(argc, argv + 1, buf);

  int pid = fork();
  if (pid == 0) {
    if (exec(prog, arglist)) {
      fprintf(2, "xargs: failed exec %s.\n", prog);
      exit(-3);
    }
  }

  int st = 0;
  wait(&st);
  if (st < 0) {
    fprintf(2, "xargs: exec %s failed.\n", prog);
    exit(-2);
  }

  exit(0);
}