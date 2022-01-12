#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);
  char buffer[15];
  if (fork() == 0) {
    read(p[0], buffer, 1);
    close(p[0]);
    printf("%d: received ping\n", getpid());
    write(p[1], "a", 1);
    close(p[1]);
  } else {
    write(p[1], "a", 1);
    close(p[1]);
    read(p[0], buffer, 1);
    close(p[0]);
    printf("%d: received pong\n", getpid());
  }

  exit(0);
}
