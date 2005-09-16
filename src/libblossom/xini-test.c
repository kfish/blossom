
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xini.h"

static int
dump_section (const char * name, void * user_data)
{
  if (!strcasecmp (name, "bollocks"))
    return XINI_SKIP_SECTION;
  printf ("[%s]\n", name);
  return XINI_OK;
}

static int
dump_assign (const char * name, const char * value, void * user_data)
{
  printf ("%s = %s\n", name, value);
  return 0;
}

int
main (int argc, char * argv[])
{
  if (argc < 1) {
    printf ("Usage: %s file\n", argv[0]);
    exit (0);
  }

  xini_read (argv[1], dump_section, NULL, dump_assign, NULL);
  
  exit (0);
}

/*
** Do not edit or modify anything in this comment block.
** The following line is a file identity tag for the GNU Arch
** revision control system.
**
** arch-tag: 09B4B282-BF88-11D8-98C7-000A95C7E2B6
*/
