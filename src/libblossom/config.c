#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "dictionary.h"

#include "private.h"
#include "xini.h"

static XiniStatus
blossom_config_section (const char * name, void * user_data)
{
  return XINI_OK;
}

static XiniStatus
blossom_config_assign (const char * name, const char * value, void * user_data)
{
  BlossomConfig * config = (BlossomConfig *) user_data;

  if (!strncmp (name, "datadir", 8)) {
    config->datadir = strdup (value);
  } else {
    dictionary_insert (config->dictionary, name, value);
  }

  return XINI_OK;
}

int
blossom_config_read (BlossomConfig * config, const char * path)
{
  XiniStatus status;

  status = xini_read (path, blossom_config_section, config,
		      blossom_config_assign, config);

  return (status == XINI_OK) ? 0 : -1;
}

BlossomConfig *
blossom_config_new (void)
{
  BlossomConfig * config;

  config = malloc (sizeof (BlossomConfig));
  config->datadir = NULL;
  config->dictionary = dictionary_new ();

  dictionary_insert (config->dictionary, "blog_title", "My weblog");

  return config;
}

int
blossom_config_delete (BlossomConfig * config)
{
  if (!config) return -1;

  dictionary_delete (config->dictionary);
  free (config);
  return 0;
}
