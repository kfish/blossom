
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "private.h"

typedef struct _BlossomInput BlossomInput;

struct _BlossomInput {
  char * path;
  time_t mtime_sec;
};

static BlossomInput *
blossom_input_new (const char * path, time_t mtime_sec)
{
  BlossomInput * bi;

  bi = (BlossomInput *) malloc (sizeof (BlossomInput));
  bi->path = x_strdup (path);
  bi->mtime_sec = mtime_sec;
  
  return bi;
}

static int
blossom_input_cmp (BlossomInput * bi1, BlossomInput * bi2)
{
  if (bi1 == NULL || bi2 == NULL) return 0;

  if (bi1->mtime_sec < bi2->mtime_sec) return -1;
  else return (bi1->mtime_sec > bi2->mtime_sec);
}

static int
blossom_input_free (BlossomInput * bi)
{
  free (bi);

  return 0;
}

int
blossom_input_init (Blossom * blossom, const char * path)
{
  blossom->path = blossom_path_new (blossom->config->datadir, path);

  blossom->input_files = x_tree_new ((x_cmp_t)blossom_input_cmp);
  blossom->input_current_node = NULL;
  blossom->input_fd = -1;
  blossom->input_offset = 0;

  blossom_path_open (blossom, blossom->path);

  return 0;
}

int
blossom_input_insert (Blossom * blossom, const char * path,
		      const struct stat * statbuf)
{
  BlossomInput * bi;

  /*OSX; bi = blossom_input_new (path, statbuf->st_mtimespec.tv_sec);*/
  bi = blossom_input_new (path, statbuf->st_mtime);
  if (bi == NULL) return -1;

  blossom->input_files = x_tree_insert (blossom->input_files, bi);

  return 0;
}

static int
blossom_input_open_current (Blossom * blossom)
{
  BlossomInput * input = x_node_data (blossom->input_files,
				      blossom->input_current_node);

  if (input == NULL) return -1;
  blossom->input_fd = open (input->path, O_RDONLY);

  if (blossom->input_fd == -1) return -1;

  blossom_datestamp (blossom, input->mtime_sec);

  return 0;
}

static int
blossom_input_close_current (Blossom * blossom)
{
  if (blossom->input_fd != -1)
    return close (blossom->input_fd);
  return 0;
}

int
blossom_input_first (Blossom * blossom)
{
  blossom->input_current_node = x_tree_first (blossom->input_files);
  blossom_input_open_current (blossom);
  return 0;
}

int
blossom_input_next (Blossom * blossom)
{
  blossom_input_close_current (blossom);

  blossom->input_current_node = x_node_next (blossom->input_files,
					     blossom->input_current_node);

  if (blossom->input_current_node == NULL) return -1;

  return blossom_input_open_current (blossom);
}


/*
** Do not edit or modify anything in this comment block.
** The following line is a file identity tag for the GNU Arch
** revision control system.
**
** arch-tag: 09A22CB2-BF88-11D8-BC5F-000A95C7E2B6
*/
