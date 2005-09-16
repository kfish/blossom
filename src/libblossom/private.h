
#ifndef __BLOSSOM_PRIVATE_H__
#define __BLOSSOM_PRIVATE_H__

#include <sys/types.h>
#include <sys/stat.h>

#include "dictionary.h"
#include "x_tree.h"

#define x_strlen(s) ((s)?strlen((s)):(0))
#define x_strdup(s) ((s)?strdup((s)):(NULL))

#undef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))

/*#define BUFFER_LEN 4096*/
#define BUFFER_LEN 8
#define VAR_LEN 256

/* Legal characters in a variable name */
#define LEGAL_VAR(c) ((isalpha((c))||isdigit((c))||c=='_'))

typedef struct _BlossomConfig BlossomConfig;
typedef struct _Blossom Blossom;

typedef enum {
  BLOSSOM_TEMPLATE_NONE = -1,
  BLOSSOM_TEMPLATE_CONTENT_TYPE = 0,
  BLOSSOM_TEMPLATE_HEAD,
  BLOSSOM_TEMPLATE_STORY,
  BLOSSOM_TEMPLATE_FOOT,
  BLOSSOM_TEMPLATE_DATE_HEAD,
  BLOSSOM_TEMPLATE_DATE_FOOT,
  BLOSSOM_TEMPLATE_END
} BlossomTemplate;

typedef enum {
  /** Not doing anything sub related */
  BLOSSOM_SUB_NONE,

  /** Scanning template variable name */
  BLOSSOM_SUB_SCAN,

  /** Copying out template variable value */
  BLOSSOM_SUB_VALUE,

  /** Copying out $body of item */
  BLOSSOM_SUB_BODY
} BlossomSubState;

struct _BlossomConfig {
  char * datadir;

  /** Dictionary of global template variables */
  Dictionary * dictionary;
};

struct _Blossom {
  BlossomConfig * config;

  /** Path that is being generated */
  char * path;

  /** Offset in generated output */
  off_t output_offset;

  /** Content type, derived from flavour */
  char * content_type;

  /* Current template state */
  BlossomTemplate template_current;

  /** Flavour template filenames */
  char * template_files[BLOSSOM_TEMPLATE_END];

  int template_fd;

  /** amount of template buffer in use */
  size_t template_used;
  int template_offset;

  /** Dictionary of path-specific template variables */
  Dictionary * dictionary;

  /** substitution handling */
  int sub_state;
  int sub_name_len;
  char * sub_value;
  char * sub_c; /* char to scan through value */
  /** substitution variable name buffer */
  char sub_name[VAR_LEN];

  /** Sorted tree of input files */
  x_tree_t * input_files;
  x_node_t * input_current_node;

  /** fd for current input story file */
  int input_fd;

  /** amount of input buffer in use */
  size_t input_used;
  int input_offset;

  /** read buffers */
  char input_buffer[BUFFER_LEN];
  char template_buffer[BUFFER_LEN];
};

int blossom_datestamp (Blossom * blossom, time_t clock);

int blossom_input_init (Blossom * blossom, const char * path);

int blossom_input_insert (Blossom * blossom, const char * path,
			  const struct stat * statbuf);
int blossom_input_first (Blossom * blossom);
int blossom_input_next (Blossom * blossom);

char * blossom_path_new (const char * path1, const char * path2);
int blossom_path_readable (const char * path);
int blossom_path_open (Blossom * blossom, const char * path);

int blossom_flavour_init (Blossom * blossom, const char * flavour);
int blossom_flavour_free (Blossom * blossom);

BlossomTemplate blossom_template_next (Blossom * blossom);
size_t blossom_template_read (Blossom * blossom, char * buf, size_t nbytes);

#endif /* __BLOSSOM_PRIVATE_H__ */

/*
** Do not edit or modify anything in this comment block.
** The following line is a file identity tag for the GNU Arch
** revision control system.
**
** arch-tag: 09AAF7F9-BF88-11D8-B909-000A95C7E2B6
*/
