#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "private.h"

#define CONTENT_TYPE_MAX_LEN 256

/* MAINT: This must be kept in sync with the BlossomTemplate enum in
 * private.h
 */
static char * template_names[BLOSSOM_TEMPLATE_END] = {
  "content_type",
  "head",
  "story",
  "foot",
  "date_head",
  "date_foot"
};
  
static char *
blossom_template_base_new (const char * template_name, const char * flavour)
{
  int l1, l2, len;
  char * template_base, * s;

  if (!template_name || !flavour) return NULL;

  l1 = strlen (template_name);
  l2 = strlen (flavour);
  len = l1 + l2 + 2;

  s = template_base = (char *) malloc (len);

  strncpy (s, template_name, l1); s += l1;
  *s = '.'; s++;
  strncpy (s, flavour, l2); s += l2;
  *s = '\0';

  return template_base;
}

static char *
blossom_template_find (Blossom * blossom, const char * template_name,
		       const char * flavour)
{
  char * scanpath = x_strdup (blossom->path);
  char * template_base, * template_path;
  char * sep;

  template_base = blossom_template_base_new (template_name, flavour);

  template_path = blossom_path_new (scanpath, template_base);
  if (blossom_path_readable (template_path)) {
    free (scanpath);
    free (template_base);
    return template_path;
  }
  free (template_path);
  template_path = NULL;

  while (*scanpath) {
    if ((sep = rindex (scanpath, '/')) != NULL) {
      *sep = '\0';
      template_path = blossom_path_new (scanpath, template_base);
      if (blossom_path_readable (template_path)) {
	free (scanpath);
	free (template_base);
	return template_path;
      } else {
	free (template_path);
	template_path = NULL;
      }
    }
  }
  
  free (scanpath);
  free (template_base);
  free (template_path);

  return NULL;
}

static int
blossom_slurp_content_type (Blossom * blossom)
{
  char * path;
  struct stat statbuf;
  char * c;
  size_t remaining, n;
  int fd;

#if 0 /* Already checked for flavour's content type */
  if (!blossom_path_readable (path))
    return NULL;
#endif

  path = blossom->template_files[BLOSSOM_TEMPLATE_CONTENT_TYPE];

  if (stat (path, &statbuf) == -1)
    return -1;

  /* Don't allow ridiculously large content types */
  if (statbuf.st_size > CONTENT_TYPE_MAX_LEN)
    return -1;

  remaining = (size_t)statbuf.st_size;
  c = blossom->content_type = (char *) malloc (remaining+1);

  if ((fd = open (path, O_RDONLY)) == -1) {
    return -1;
  }

  while (remaining > 0) {
    n = read (fd, c, remaining);
    remaining -= n;
    c += n;
  }
  *c = '\0';

  close (fd);

  return 0;
}

int
blossom_flavour_init (Blossom * blossom, const char * flavour)
{
  int i;

  blossom->template_current = BLOSSOM_TEMPLATE_NONE;
  blossom->template_offset = 0;
  blossom->template_used = 0;

  for (i = 0; i < BLOSSOM_TEMPLATE_END; i++) {
    blossom->template_files[i] =
      blossom_template_find (blossom, template_names[i], flavour);
  }

  blossom->template_fd = -1;

  blossom_slurp_content_type (blossom);

  return 0;
}

int
blossom_flavour_free (Blossom * blossom)
{
  int i;

  for (i = 0; i < BLOSSOM_TEMPLATE_END; i++) {
    free (blossom->template_files[i]);
  }

  free (blossom->content_type);

  return 0;
}
