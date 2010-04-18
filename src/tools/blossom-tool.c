#include <stdio.h>
#include <stdlib.h>
#include <blossom/blossom.h>

#define BUFFER_LEN 12

#define DEFAULT_INI_FILE "/tmp/blossom.ini"

int
main (int argc, char * argv[])
{
  BlossomConfig * config;
  Blossom * blossom;
  char * progname;
  char * path;
  char buf[BUFFER_LEN];
  ssize_t n;
  int status;

  progname = argv[0];

  if (argc < 2) {
    path = NULL;
  } else {
    path = argv[1];
  }

  config = blossom_config_new ();
  status = blossom_config_read (config, DEFAULT_INI_FILE);

  if (status == -1) {
    fprintf (stderr, "%s: error reading %s\n", progname, DEFAULT_INI_FILE);
    goto exit_err;
  }

  blossom = blossom_open (config, path, "html");

  /*printf ("Content-Type: %s\r\n", blossom_get_content_type (blossom));*/

  while ((n = blossom_read (blossom, buf, BUFFER_LEN)) > 0) {
    fwrite (buf, 1, n, stdout);
    /*printf ("[blossom-tool] Read %d bytes\n", n);*/
  }

  blossom_close (blossom);

  blossom_config_delete (config);

  return 0;

exit_err:
  blossom_config_delete (config);
  return 1;
}
