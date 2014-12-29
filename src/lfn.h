/*
 * In The Name Of God
 * ========================================
 * [] File Name : lfn.h
 *
 * [] Creation Date : 27-12-2014
 *
 * [] Last Modified : Mon 29 Dec 2014 11:19:09 AM IRST
 *
 * [] Created By : Parham Alvani (parham.alvani@gmail.com)
 * =======================================
*/
#ifndef _LFN_H
#define _LFN_H

#include <stdint.h>

#include "FAT.h"

/*
 * According to
 * http://en.wikipedia.org/wiki/Design_of_the_FAT_file_system
 * Sequence Number (id):
 * bit 6: last logical, first physical LFN entry
 * bit 5: 0
 * bits 4-0: number 0x01..0x14 (0x1F), deleted entry: 0xE5
 *
*/
#define LFN_ID_START	0x40
#define LFN_ID_SLOTMASK	0x1F

#define VFAT_LN_ATTR	0x0F

#define CHARS_PER_LFN	13

/*
 * @id: sequence number for slot
 * @name0_4: first 5 characters in name
 * @attr: attribute byte
 * @reserved: always 0
 * @alias_checksum: checksum for 8.3 alias
 * @name5_10: 6 more characters in name
 * @start: starting cluster number, 0 in long slots
 * @name11_12: last 2 characters in name
*/

struct fat_dir_long_name {
	uint8_t		id;
	uint8_t		name0_4[10];
	uint8_t		attr;
	uint8_t		reserved;
	uint8_t		alias_checksum;
	uint8_t		name5_10[12];
	uint16_t	start;
	uint8_t		name11_12[4];
};


/*
 * Reset the state of the LFN parser.
*/
void lfn_reset(void);

/*
 * Process a dir slot that is a VFAT LFN entry.
*/
void lfn_add_slot(const struct fat_dir_layout *de, int dir_offset);

/*
 * Retrieve the long name for the proper dir entry.
*/
char *lfn_get(const struct fat_dir_layout *de, int *lfn_offset);

/*
 * is LFN (Long File Name)
*/
int is_lfn(const uint8_t attr);

void lfn_check_orphaned(void);

#endif
