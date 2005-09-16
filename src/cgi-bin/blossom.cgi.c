#include <stdio.h>
#include <stdlib.h>
#include <blossom/blossom.h>

#define BUFFER_LEN 12

int
main (int argc, char * argv[])
{
  BlossomConfig * config;
  Blossom * blossom;
  char * path;
  char buf[BUFFER_LEN];
  ssize_t n;

  path = getenv ("PATH_INFO");

  config = blossom_config_new ();
  blossom_config_read (config, "/tmp/blossom.ini");

  blossom = blossom_open (config, path, "html");

  printf ("Content-Type: %s\r\n", blossom_get_content_type (blossom));

  while ((n = blossom_read (blossom, buf, BUFFER_LEN)) > 0) {
    fwrite (buf, 1, n, stdout);
    /*printf ("[blossom-tool] Read %d bytes\n", n);*/
  }

  blossom_close (blossom);

  return 0;
}

/*
** Do not edit or modify anything in this comment block.
** The following line is a file identity tag for the GNU Arch
** revision control system.
**
** arch-tag: 0975B807-BF88-11D8-89C5-000A95C7E2B6
*/
