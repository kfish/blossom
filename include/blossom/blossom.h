/*
 *
 */

#ifndef __BLOSSOM_H__
#define __BLOSSOM_H__

#include <sys/types.h>

typedef void BlossomConfig;
typedef void Blossom;

BlossomConfig *
blossom_config_new (void);

int
blossom_config_read (BlossomConfig * config, const char * path);

int
blossom_config_delete (BlossomConfig * config);

Blossom *
blossom_open (BlossomConfig * config, const char * subpath, 
	      const char * flavour);

ssize_t
blossom_read (Blossom * blossom, void * buf, size_t count);

int
blossom_close (Blossom * blossom);

const char *
blossom_get_content_type (Blossom * blossom);

#endif /* __BLOSSOM_H__ */

/*
** Do not edit or modify anything in this comment block.
** The following line is a file identity tag for the GNU Arch
** revision control system.
**
** arch-tag: 096D5D28-BF88-11D8-8423-000A95C7E2B6
*/
