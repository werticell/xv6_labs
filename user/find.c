#include "kernel/types.h" // uint usage in user.h
#include "kernel/stat.h" // stat struct
#include "kernel/fs.h" // dirent struct

#include "user.h"


// From user/ls.c
char* fmtname(char *path) {
  static char buf[DIRSIZ+1];
  char *p;
  // Find first character after last slash.
  for (p=path+strlen(path); p >= path && *p != '/'; p--);
  p++;
  // Return blank-padded name.
  int p_len = strlen(p);
  if (p_len >= DIRSIZ)
    return p;
  memmove(buf, p, p_len);
  memset(buf + p_len, ' ', DIRSIZ - p_len);
  return buf;
}


void find(char *path, char *file_name) {
  char buf[512];
  char *p;
  int fd = 0;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }
  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }
  switch (st.type) {
    case T_FILE:
      if (strcmp(file_name, fmtname(path))) {
        printf("%s\n", path);
      }
      break;
    case T_DIR:
      if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
        printf("find: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';
      while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) {
          continue;
        }
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        int name_len = strlen(de.name);
        if ((name_len == 1 && de.name[0] == '.') || (name_len == 2 && de.name[0] == '.' && de.name[1] == '.')) {
          continue;
        }
        find(buf, file_name);
      }
      break;
  }
  close(fd);
}

int main(int argc, char *argv[]) {
  // find <path> <file_name>
  if(argc >= 3) {
    find(argv[1], argv[2]);
  }
  exit(0);
}
