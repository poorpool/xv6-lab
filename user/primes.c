#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);
  if (fork()) {
    close(p[0]);
    for (int i = 2; i <= 35; i++) {
      write(p[1], &i, 4);
    }
    close(p[1]);
    wait((int *)0);
  } else {
    while (1) {
      close(p[1]);
      int r_fd = p[0];
      int first = 0, tmp;
      read(r_fd, &first, 4);  // must success
      printf("prime %d\n", first);
      if (read(r_fd, &tmp, 4) == 0) {
        close(p[0]);
        break;
      } else {
        pipe(p);
        if (fork()) {
          close(p[0]);
          do {
            if (tmp % first)
              write(p[1], &tmp, 4);
          } while (read(r_fd, &tmp, 4));
          close(r_fd);
          close(p[1]);
          wait((int *)0);
          break;
        }
      }
    }
  }
  exit(0);
}
