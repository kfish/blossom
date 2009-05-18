#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h> /* isalpha etc. */
#include <string.h> /* strncmp */

#include "private.h"

/*#define DEBUG*/

#ifdef DEBUG
#include <stdio.h>
#endif

static int
blossom_template_set (Blossom * blossom, BlossomTemplate template)
{
  blossom->template_current = template;

  if (template == BLOSSOM_TEMPLATE_END)
    blossom->template_fd = -1;
  else
    blossom->template_fd = open (blossom->template_files[template], O_RDONLY);

  blossom->template_used = 0;
  blossom->template_offset = 0;

  blossom->sub_state = BLOSSOM_SUB_NONE;
  blossom->sub_name_len = 0;
  blossom->sub_value = NULL;
  blossom->sub_c = NULL;
  blossom->sub_name[0] = '\0';

  return blossom->template_fd;
}

BlossomTemplate
blossom_template_next (Blossom * blossom)
{
  if (blossom->template_fd != -1) {
    close (blossom->template_fd);
    blossom->template_fd = -1;
  }

  while (blossom->template_fd == -1 &&
	 blossom->template_current != BLOSSOM_TEMPLATE_END) {
    switch (blossom->template_current) {
    case BLOSSOM_TEMPLATE_NONE:
      blossom_template_set (blossom, BLOSSOM_TEMPLATE_HEAD);
      break;
    case BLOSSOM_TEMPLATE_CONTENT_TYPE:
      blossom_template_set (blossom, BLOSSOM_TEMPLATE_HEAD);
      break;
    case BLOSSOM_TEMPLATE_HEAD:
      blossom_template_set (blossom, BLOSSOM_TEMPLATE_DATE_HEAD);

      /* Load the first input file */
      blossom_input_first (blossom);
      break;
    case BLOSSOM_TEMPLATE_DATE_HEAD:
      blossom_template_set (blossom, BLOSSOM_TEMPLATE_STORY);
      break;
    case BLOSSOM_TEMPLATE_STORY:
      blossom_template_set (blossom, BLOSSOM_TEMPLATE_DATE_FOOT);

      break;
    case BLOSSOM_TEMPLATE_DATE_FOOT:
      /* Check for next input file */
      if (blossom_input_next (blossom) != -1) {
	blossom_template_set (blossom, BLOSSOM_TEMPLATE_DATE_HEAD);
      } else {
	blossom_template_set (blossom, BLOSSOM_TEMPLATE_FOOT);
      }
      break;
    case BLOSSOM_TEMPLATE_FOOT:
    default:
      blossom_template_set (blossom, BLOSSOM_TEMPLATE_END);
      break;
    }
  }

  return blossom->template_current;
}

static int
blossom_template_fill (Blossom * blossom)
{
  blossom->template_used =
    read (blossom->template_fd, blossom->template_buffer, BUFFER_LEN);
  blossom->template_offset = 0;

  return blossom->template_used;
}

static int
blossom_sub_value (Blossom * blossom)
{
  /* Look up cached value in local dictionary */
  blossom->sub_value = (char *)
    dictionary_lookup (blossom->dictionary, blossom->sub_name);

  /* If not found in the local dictionary, try the global one */
  if (!blossom->sub_value) {
    blossom->sub_value = (char *)
      dictionary_lookup (blossom->config->dictionary, blossom->sub_name);
  }

  blossom->sub_c = blossom->sub_value;

  blossom->sub_state = BLOSSOM_SUB_VALUE;

  return 0;
}

static void
blossom_sub_reset (Blossom * blossom)
{
  blossom->sub_name_len = 0;
  blossom->sub_name[0] = '\0';

  /*if (blossom->sub_value) free (blossom->sub_value);*/
  blossom->sub_value = NULL;
  blossom->sub_c = NULL;

  blossom->sub_state = BLOSSOM_SUB_NONE;
}

static size_t
blossom_template_sub (Blossom * blossom, char * buf, size_t nbytes)
{
  size_t remaining = nbytes, nout = 0, n;
  char *c = &blossom->template_buffer[blossom->template_offset];

  while (remaining > 0 &&
	 (blossom->template_offset < blossom->template_used)) {
    switch (blossom->sub_state) {
    case BLOSSOM_SUB_NONE:
      if (*c == '$') {
	blossom->sub_state = BLOSSOM_SUB_SCAN;
      } else {
	*buf++ = *c;
	nout++;
	remaining--;
      }
      c++; blossom->template_offset++;
      break;
    case BLOSSOM_SUB_SCAN:
      if (LEGAL_VAR(*c) && blossom->sub_name_len < VAR_LEN-1) {
	blossom->sub_name[blossom->sub_name_len++] = *c;
	c++; blossom->template_offset++;
      } else {
	blossom->sub_name[blossom->sub_name_len++] = '\0';
	if (!strncmp (blossom->sub_name, "body", 5)) {
	  blossom->sub_state = BLOSSOM_SUB_BODY;
	} else {
	  blossom_sub_value (blossom);
	}
      }
      break;
    case BLOSSOM_SUB_VALUE:
      {
	if (blossom->sub_c && *blossom->sub_c) {
	  *buf++ = *blossom->sub_c;
	  blossom->sub_c++;
	  nout++;
	  remaining--;
	} else {
	  blossom_sub_reset (blossom);
	} 
      }
      break;
    case BLOSSOM_SUB_BODY:
      n = read (blossom->input_fd, buf, remaining);
      if (n == -1) {
      } else if (n == 0) {
	/* when finished copying out body, reset */
	blossom_sub_reset (blossom);
      } else {
	buf += n;
	nout += n;
	remaining -= n;
      }
      break;
    }
  }

  if (nout > nbytes) abort();

  return nout;
}

size_t
blossom_template_read (Blossom * blossom, char * buf, size_t nbytes)
{
  size_t remaining = nbytes, nread = 0, n;

  /* If there's no template for the current state, return as for EOF.
   * The state transition above ensures that either a state with valid
   * template is found, or the state is set to BLOSSOM_TEMPLATE_END */
  if (blossom->template_fd == -1) {
#ifdef DEBUG
    printf ("*** [blossom_template_read] template_fd == -1, ret 0\n");
#endif
    return 0;
  }

  while (remaining > 0) {
    if (blossom->template_offset >= blossom->template_used)
      blossom_template_fill (blossom);

    if (blossom->template_used == 0) goto out;

    n = blossom_template_sub (blossom, buf, remaining);

    remaining -= n;
    buf += n;
    nread += n;
  }

 out:
#ifdef DEBUG
  if (nread == 0) {
    printf ("*** [blossom_template_read] read 0, ret 0\n");
  }
#endif
    
  return nread;
}
