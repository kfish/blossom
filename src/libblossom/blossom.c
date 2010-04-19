#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "private.h"

Blossom *
blossom_open (BlossomConfig * config, const char * path, const char * flavour)
{
  Blossom * blossom = NULL;
  const char * p = path;

  if (path == NULL)
	  p = getcwd(NULL, 0);

  blossom = (Blossom *) malloc (sizeof (Blossom));

  /* Attach config */
  blossom->config = config;

  /* Initialize */
  blossom->dictionary = dictionary_new ();

  blossom_input_init (blossom, p);
  blossom_flavour_init (blossom, flavour);

  if (path == NULL)
	  free (p);

  return blossom;
}

ssize_t
blossom_read (Blossom * blossom, char * buf, size_t count)
{
  ssize_t remaining = count, n, nread = 0;
  int active = 1;

  while (active && remaining > 0) {
    n = blossom_template_read (blossom, buf, remaining);
    if (n == 0) {
      /* Move on to the next template state */
      if (blossom_template_next (blossom) == BLOSSOM_TEMPLATE_END) {
	active = 0;
      }
    }
    
    remaining -= n;
    buf += n;
    nread += n;
  }

  return nread;
}

int
blossom_close (Blossom * blossom)
{
  dictionary_delete (blossom->dictionary);
  blossom_input_delete (blossom);
  blossom_flavour_free (blossom);
  free (blossom);

  return 0;
}

const char *
blossom_get_content_type (Blossom * blossom)
{
  return blossom->content_type;
}
