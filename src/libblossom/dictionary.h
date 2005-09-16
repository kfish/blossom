
#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

typedef void Dictionary;

Dictionary * dictionary_new (void);
int dictionary_insert (Dictionary * table, const char * name,
		       const char * value);
const char * dictionary_lookup (Dictionary * table, const char * name);
int dictionary_delete (Dictionary * table);

#endif /* __DICTIONARY_H__ */

/*
** Do not edit or modify anything in this comment block.
** The following line is a file identity tag for the GNU Arch
** revision control system.
**
** arch-tag: 09833823-BF88-11D8-ADE7-000A95C7E2B6
*/
