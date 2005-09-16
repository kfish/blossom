#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include "private.h"

Blossom *
blossom_open (BlossomConfig * config, const char * path, const char * flavour)
{
  Blossom * blossom = NULL;

  blossom = (Blossom *) malloc (sizeof (Blossom));

  /* Attach config */
  blossom->config = config;

  /* Initialize */
  blossom->dictionary = dictionary_new ();

  blossom_input_init (blossom, path);
  blossom_flavour_init (blossom, flavour);

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
  free (blossom);

  return 0;
}

const char *
blossom_get_content_type (Blossom * blossom)
{
  return blossom->content_type;
}

/*
** Do not edit or modify anything in this comment block.
** The following line is a file identity tag for the GNU Arch
** revision control system.
**
** arch-tag: 099004F2-BF88-11D8-B8CA-000A95C7E2B6
*/
