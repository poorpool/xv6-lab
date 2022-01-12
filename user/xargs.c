#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

char buffer[1919];
int len;

void chuli(int argc, char *argv[]) {
  char *argvs[MAXARG+1];
  for (int i = 1; i < argc; i++) {
    argvs[i-1] = argv[i];
  }
  argvs[argc-1] = buffer;
  argvs[argc] = (char *)0;
  if (fork()) {
    wait((int *)0);
  } else {
    exec(argv[1], argvs);
  }
}

int
main(int argc, char *argv[])
{
  while (read(0, buffer+len, 1)) {
    len++;
    if (buffer[len-1] == '\n') {
      buffer[--len] = '\0';
      chuli(argc, argv);
      len = 0;
    }
  }
  if (len) {
    buffer[len] = '\0';
    chuli(argc, argv);
  }

  exit(0);
}
