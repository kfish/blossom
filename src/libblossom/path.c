#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "private.h"

/* concatenate two path components, returning a newly malloced string */
char *
blossom_path_new (const char * path1, const char * path2)
{
  int l1, l2, len;
  char * new_path, * s;

  l1 = x_strlen (path1);
  l2 = x_strlen (path2);
  len = l1 + l2 + 2;

  s = new_path = (char *) malloc (len);

  if (path1) {
    strncpy (s, path1, l1);
    s += l1;

    /* Backtrack s if path1 ends in '/' */
    if (*(s-1) == '/') s--;
  }

  /* Only insert the slash if it wouldn't appear otherwise */
  if ((path1 && !path2) || *path2 != '/') {
    *s = '/';
    s++;
  }

  if (path2) {
    strncpy (s, path2, l2);
    s += l2;
  }

  *s = '\0';

  return new_path;
}

int
blossom_path_readable (const char * path)
{
  struct stat statbuf;

  if (stat (path, &statbuf) == -1) return 0;

  if (!S_ISREG(statbuf.st_mode) && !S_ISLNK(statbuf.st_mode)) return 0;

  if (!(statbuf.st_mode & S_IRUSR)) return 0;

  return 1;
}

int
blossom_path_open (Blossom * blossom, const char * path)
{
  struct stat statbuf;
  DIR * dir;
  struct dirent * ent;
  char * new_path = NULL;
  char * ext;

  if (stat (path, &statbuf) == -1) {
    /* XXX: error */
    return -1;
  }

  if (!S_ISDIR(statbuf.st_mode)) {
    return -1;
  }

  dir = opendir (path);

  while ((ent = readdir (dir)) != NULL) {
    /* Don't re-read ., and don't follow .. */
    if (!strcmp (ent->d_name, ".") || !strcmp (ent->d_name, ".."))
      continue;

    new_path = blossom_path_new (path, ent->d_name);

    if (stat (new_path, &statbuf) == -1) {
      /* XXX: error */
      goto skip;
    }

    /* If the path is a regular .txt file, insert it */
    if (S_ISREG(statbuf.st_mode) || S_ISLNK(statbuf.st_mode)) {
      if (((ext = rindex (new_path, '.')) != NULL) &&
	  !strncmp (ext, ".txt", 4))
	blossom_input_insert (blossom, new_path, &statbuf);
    }

    /* If the path is a directory, crawl it */
    if (S_ISDIR(statbuf.st_mode)) {
      blossom_path_open (blossom, new_path);
    }

  skip:
    free (new_path);
  }

  closedir (dir);

  return 0;
}
