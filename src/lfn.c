#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#include "common.h"
#include "lfn.h"
#include "FAT.h"


/*
 * These global vars represent the state of the LFN parser
*/
unsigned char *lfn_unicode = NULL;
unsigned char lfn_checksum;
int lfn_slot = -1;
int *lfn_offsets = NULL;
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
#define UNICODE_CONVERTABLE(cl, ch)	(ch == 0 && (cl < 0x80 || cl >= 0xa0))

/* for maxlen param */
#define UNTIL_0		INT_MAX

/* Convert name part in 'lfn' from unicode to ASCII */
#define CNV_THIS_PART(lfn)					\
	do {							\
		unsigned char __part_uni[CHARS_PER_LFN * 2];	\
		copy_lfn_part(__part_uni, lfn);			\
		cnv_unicode(__part_uni, CHARS_PER_LFN, 0);	\
	} while (0)

/* Convert name parts collected so far (from previous slots) from unicode to
 * ASCII */
#define CNV_PARTS_SO_FAR()					\
	cnv_unicode(lfn_unicode +				\
			(lfn_slot*CHARS_PER_LFN*2),		\
			lfn_parts*CHARS_PER_LFN, 0)		\

/* Concat two characters to form a wide character*/
#define BYTES_TO_WCHAR(ch1, ch2)				\
	((wchar_t)((unsigned)(ch1) + ((unsigned)(ch2) << 8)))	\

/*
 * This is a helper function for calculate number of multibyte characters
 * that wide character needed when converted to multibyte form.
*/
static size_t mbslen(wchar_t x)
{
	wchar_t wstr[] = {x, 0};

	return wcstombs(NULL, wstr, 0);
}

static size_t wctombs(char *dest, wchar_t x)
{
	wchar_t wstr[] = {x, 0};
	size_t size = wcstombs(NULL, wstr, 0);

	if (size != (size_t) -1)
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
		if (mbslen(BYTES_TO_WCHAR(up[0], up[1])) != (size_t) -1) {
			x = mbslen(BYTES_TO_WCHAR(up[0], up[1]));
			len += x;
		} else if (UNICODE_CONVERTABLE(up[0], up[1])) {
			++len;
		} else {
			len += 4;
		}
	}
	cp = out = malloc(len + 1);

	for (up = uni; (up - uni) / 2 < maxlen && (up[0] || up[1]); up += 2) {
		if (wctombs((char *)cp, BYTES_TO_WCHAR(up[0], up[1])) !=
				(size_t) -1) {
			x = wctombs((char *)cp, BYTES_TO_WCHAR(up[0], up[1]));
			cp += x;
		} else if (UNICODE_CONVERTABLE(up[0], up[1])) {
			*cp++ = up[0];
		} else {
			/*
			 * here the same escape notation
			 * is used as in the Linux kernel
			*/
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

/*
 * This function copy name parts of fat_dir_long_name
 * in continuous form into dst
*/
static void copy_lfn_part(unsigned char *dst, struct fat_dir_long_name *lfn)
{
	memcpy(dst, lfn->name0_4, 10);
	memcpy(dst + 10, lfn->name5_10, 12);
	memcpy(dst + 22, lfn->name11_12, 4);
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
void lfn_add_slot(const struct fat_dir_layout *de, int dir_offset)
{
	struct fat_dir_long_name *lfn = (struct fat_dir_long_name *) de;
	int slot = lfn->id & LFN_ID_SLOTMASK;
	unsigned int offset;

	if (lfn_slot == 0)
		lfn_check_orphaned();
	if (de->attr != VFAT_LN_ATTR)
		udie("lfn_add_slot called with non-LFN directory entry");
	if (lfn->id & LFN_ID_START && slot != 0) {
		lfn_slot = slot;
		lfn_checksum = lfn->alias_checksum;
		lfn_unicode = malloc((lfn_slot * CHARS_PER_LFN + 1) * 2);
		lfn_offsets = malloc(lfn_slot * sizeof(int));
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
char *lfn_get(const struct fat_dir_layout *de, int *lfn_offset)
{
	char *lfn;

	*lfn_offset = 0;
	if (de->attr == VFAT_LN_ATTR)
		udie("lfn_get called with LFN directory entry");

	if (lfn_slot == -1)
		/* no long name for this file */
		goto error;

	if (lfn_slot != 0)
		/* The long name isn't finished yet. */
		/* die("lfn_get called when long name isn't finished."); */
		goto error;
	*lfn_offset = lfn_offsets[0];
	lfn = cnv_unicode(lfn_unicode, UNTIL_0, 1);
	lfn_reset();
	return lfn;
error:
	lfn_reset();
	return NULL;
}

int is_lfn(uint8_t const attr)
{
	return (attr == VFAT_LN_ATTR);
}

void lfn_check_orphaned(void)
{
	char *long_name;

	if (lfn_slot == -1)
		return;

	long_name = CNV_PARTS_SO_FAR();
	printf("Orphaned long file name part \"%s\"\n", long_name);

	lfn_reset();
}
