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

  if (argc < 2) {
    path = NULL;
  } else {
    path = argv[1];
  }

  config = blossom_config_new ();
  blossom_config_read (config, "/tmp/blossom.ini");

  blossom = blossom_open (config, path, "html");

  /*printf ("Content-Type: %s\r\n", blossom_get_content_type (blossom));*/

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
** arch-tag: 09CC0BCC-BF88-11D8-8694-000A95C7E2B6
*/
