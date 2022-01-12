#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int nowLen;
char nowName[4555];

void dfs(char *name) {
  int fd = open(nowName, 0);
  struct stat st;
  fstat(fd, &st);
  struct dirent de;

  switch (st.type) {
  case T_FILE:
    printf("%s\n", nowName);
    break;
  case T_DIR:
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
        continue;
      }
      int tmplen = nowLen;
      strcpy(nowName+nowLen, de.name);
      nowLen = strlen(nowName);
      stat(nowName, &st);
      if (st.type == T_FILE && strcmp(de.name, name) == 0) {
        dfs(name);
      } else if (st.type == T_DIR) {
        nowName[nowLen++] = '/';
        nowName[nowLen] = '\0';
        dfs(name);
      }
      nowLen = tmplen;
      nowName[nowLen] = '\0';
    }
    break;
  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(3, "Usage: find directory name\n");
    exit(1);
  }

  strcpy(nowName, argv[1]);
  nowLen = strlen(nowName);
  if (nowName[nowLen-1] != '/') {
    nowName[nowLen++] = '/';
    nowName[nowLen] = '\0';
  }
  dfs(argv[2]);

  exit(0);
}
