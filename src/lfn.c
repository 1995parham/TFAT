#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#include "common.h"
#include "lfn.h"
#include "FAT.h"


#define CHARS_PER_LFN	13

/*
 * These modul-global vars represent the state of the LFN parser
*/
unsigned char *lfn_unicode = NULL;
unsigned char lfn_checksum;
int lfn_slot = -1;
loff_t *lfn_offsets = NULL;
int lfn_parts = 0;

static unsigned char fat_uni2esc[64] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
    'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
    'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z', '+', '-'
};

/* This defines which unicode chars are directly convertable to ISO-8859-1 */
#define UNICODE_CONVERTABLE(cl,ch)	(ch == 0 && (cl < 0x80 || cl >= 0xa0))

/* for maxlen param */
#define UNTIL_0		INT_MAX

/* Convert name part in 'lfn' from unicode to ASCII */
#define CNV_THIS_PART(lfn)				\
    ({							\
	unsigned char __part_uni[CHARS_PER_LFN*2];		\
	copy_lfn_part( __part_uni, lfn );		\
	cnv_unicode( __part_uni, CHARS_PER_LFN, 0 );	\
    })

/* Convert name parts collected so far (from previous slots) from unicode to
 * ASCII */
#define CNV_PARTS_SO_FAR()					\
	(cnv_unicode( lfn_unicode+(lfn_slot*CHARS_PER_LFN*2),	\
		      lfn_parts*CHARS_PER_LFN, 0 ))

#define BYTES_TO_WCHAR(cl,ch) ((wchar_t)((unsigned)(cl) + ((unsigned)(ch) << 8)))
static size_t mbslen(wchar_t x)
{
    wchar_t wstr[] = { x, 0 };
    return wcstombs(NULL, wstr, 0);
}

static size_t wctombs(char *dest, wchar_t x)
{
    wchar_t wstr[] = { x, 0 };
    size_t size = wcstombs(NULL, wstr, 0);
    if (size != (size_t) - 1)
	size = wcstombs(dest, wstr, size + 1);
    return size;
}

/*
 * This function converts an unicode string to a normal ASCII string, assuming
 * ISO-8859-1 charset. Characters not in 8859-1 are converted to the same
 * escape notation as used by the kernel, i.e. the uuencode-like ":xxx"
*/
static char *cnv_unicode(const unsigned char *uni, int maxlen, int use_q)
{
    const unsigned char *up;
    unsigned char *out, *cp;
    int len, val;
    size_t x;

    for (len = 0, up = uni; (up - uni) / 2 < maxlen && (up[0] || up[1]);
	 up += 2) {
	if ((x = mbslen(BYTES_TO_WCHAR(up[0], up[1]))) != (size_t) - 1)
	    len += x;
	else if (UNICODE_CONVERTABLE(up[0], up[1]))
	    ++len;
	else
	    len += 4;
    }
    cp = out = use_q ? qalloc(&mem_queue, len + 1) : alloc(len + 1);

    for (up = uni; (up - uni) / 2 < maxlen && (up[0] || up[1]); up += 2) {
	if ((x =
	     wctombs((char *)cp, BYTES_TO_WCHAR(up[0], up[1]))) != (size_t) - 1)
	    cp += x;
	else if (UNICODE_CONVERTABLE(up[0], up[1]))
	    *cp++ = up[0];
	else {
	    /* here the same escape notation is used as in the Linux kernel */
	    *cp++ = ':';
	    val = (up[1] << 8) + up[0];
	    cp[2] = fat_uni2esc[val & 0x3f];
	    val >>= 6;
	    cp[1] = fat_uni2esc[val & 0x3f];
	    val >>= 6;
	    cp[0] = fat_uni2esc[val & 0x3f];
	    cp += 3;
	}
    }
    *cp = 0;

    return (char *)out;
}

static void copy_lfn_part(unsigned char *dst, fat_dir_long_name *lfn)
{
    memcpy(dst, lfn->name0_4, 10);
    memcpy(dst + 10, lfn->name5_10, 12);
    memcpy(dst + 22, lfn->name11_12, 4);
}

static void clear_lfn_slots(int start, int end)
{
    int i;
    LFN_ENT empty;

    /* New dir entry is zeroed except first byte, which is set to 0xe5.
     * This is to avoid that some FAT-reading OSes (not Linux! ;) stop reading
     * a directory at the first zero entry...
     */
    memset(&empty, 0, sizeof(empty));
    empty.id = DELETED_FLAG;

    for (i = start; i <= end; ++i) {
	fs_write(lfn_offsets[i], sizeof(LFN_ENT), &empty);
    }
}

void lfn_fix_checksum(loff_t from, loff_t to, const char *short_name)
{
    int i;
    __u8 sum;
    for (sum = 0, i = 0; i < 11; i++)
	sum = (((sum & 1) << 7) | ((sum & 0xfe) >> 1)) + short_name[i];

    for (; from < to; from += sizeof(LFN_ENT)) {
	fs_write(from + offsetof(LFN_ENT, alias_checksum), sizeof(sum), &sum);
    }
}

void lfn_reset(void)
{
    if (lfn_unicode)
	free(lfn_unicode);
    lfn_unicode = NULL;
    if (lfn_offsets)
	free(lfn_offsets);
    lfn_offsets = NULL;
    lfn_slot = -1;
}

/*
 * This function is only called with de->attr == VFAT_LN_ATTR. It stores part
 * of the long name.
*/
void lfn_add_slot(fat_dir_layout *de, int dir_offset)
{
    	fat_dir_long_name *lfn = (fat_dir_long_name *) de;
    	int slot = lfn->id & LFN_ID_SLOTMASK;
    	unsigned int offset;

    	if (lfn_slot == 0)
		lfn_check_orphaned();
    	if (de->attr != VFAT_LN_ATTR)
		die("lfn_add_slot called with non-LFN directory entry");
	if (lfn->id & LFN_ID_START && slot != 0) {
		lfn_slot = slot;
		lfn_checksum = lfn->alias_checksum;
		lfn_unicode = malloc((lfn_slot * CHARS_PER_LFN + 1) * 2);
		lfn_offsets = malloc(lfn_slot * sizeof(loff_t));
		lfn_parts = 0;
	}
    	if (lfn_slot != -1) {
		lfn_slot--;
		offset = lfn_slot * CHARS_PER_LFN * 2;
		copy_lfn_part(lfn_unicode + offset, lfn);
		if (lfn->id & LFN_ID_START)
			lfn_unicode[offset + 26] = lfn_unicode[offset + 27] = 0;
		lfn_offsets[lfn_parts++] = dir_offset;
    	}
}

/* 
 * This function is always called when de->attr != VFAT_LN_ATTR is found, to
 * retrieve the previously constructed LFN.
*/
char *lfn_get(fat_dir_layout *de, int *lfn_offset)
{
    	char *lfn;
    	__u8 sum;
    	int i;

    	*lfn_offset = 0;
    	if (de->attr == VFAT_LN_ATTR)
		die("lfn_get called with LFN directory entry");

    	if (lfn_slot == -1)
		/* no long name for this file */
		return NULL;

    	if (lfn_slot != 0) {
		/* The long name isn't finished yet. */
	}

    for (sum = 0, i = 0; i < 8; i++)
	sum = (((sum & 1) << 7) | ((sum & 0xfe) >> 1)) + de->name[i];
    for (i = 0; i < 3; i++)
	sum = (((sum & 1) << 7) | ((sum & 0xfe) >> 1)) + de->ext[i];
    if (sum != lfn_checksum) {
	/* checksum doesn't match, long name doesn't apply to this alias */
	/* Causes: 1) alias renamed */
	/* Fixes: 1) Fix checksum in LFN entries */
	char *long_name = CNV_PARTS_SO_FAR();
	char *short_name = file_name(de->name);
	printf("Wrong checksum for long file name \"%s\".\n"
	       "  (Short name %s may have changed without updating the long name)\n",
	       long_name, short_name);
	free(long_name);
	if (interactive) {
	    printf("1: Delete LFN\n2: Leave it as it is.\n"
		   "3: Fix checksum (attaches to short name %s)\n", short_name);
	} else
	    printf("  Not auto-correcting this.\n");
	if (interactive) {
	    switch (get_key("123", "?")) {
	    case '1':
		clear_lfn_slots(0, lfn_parts - 1);
		lfn_reset();
		return NULL;
	    case '2':
		lfn_reset();
		return NULL;
	    case '3':
		for (i = 0; i < lfn_parts; ++i) {
		    fs_write(lfn_offsets[i] + offsetof(LFN_ENT, alias_checksum),
			     sizeof(sum), &sum);
		}
		break;
	    }
	}
    }

    *lfn_offset = lfn_offsets[0];
    lfn = cnv_unicode(lfn_unicode, UNTIL_0, 1);
    lfn_reset();
    return (lfn);
}

void lfn_check_orphaned(void)
{
    char *long_name;

    if (lfn_slot == -1)
	return;

    long_name = CNV_PARTS_SO_FAR();
    printf("Orphaned long file name part \"%s\"\n", long_name);
    if (interactive)
	printf("1: Delete.\n2: Leave it.\n");
    else
	printf("  Auto-deleting.\n");
    if (!interactive || get_key("12", "?") == '1') {
	clear_lfn_slots(0, lfn_parts - 1);
    }
    lfn_reset();
}
