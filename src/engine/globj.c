#include "globj.h"
#include "engine.h"

INAT bglobjsrch(struct hz_bglobj *o, INAT l, INAT r, U64 x) 
{ 
	if (r >= l) { 
		INAT mid = l + (r - l) / 2; 
		if (o[mid].namehash == x) return mid; 
		if (o[mid].namehash > x) return bglobjsrch(o, l, mid - 1, x); 
		return bglobjsrch(o, mid + 1, r, x); 
	}
	return -1; 
} 

struct hz_bglobj get_bglobj(struct hz_bglobj *objlist, UNAT objlist_count, U64 snmid)
{
	struct hz_bglobj target;
	INAT idx = bglobjsrch(objlist, 0, objlist_count - 1, snmid);
	if (idx < 0) target.failed = true;
	else target = objlist[idx];

	return target;
}

X0 sort_bglobj_list(struct hz_bglobj *objlist, UNAT objlist_count)
{
	INAT i, j, loindex; 
	if (objlist_count < 2) return;

	for (i = 0; i < objlist_count - 1; i++) { 
		loindex = i; 
		for (j = i + 1; j < objlist_count; j++) 
			if (objlist[j].namehash < objlist[loindex].namehash) loindex = j; 

		SWAP(objlist[loindex], objlist[i]); 
	} 
}

X0 add_new_bglobj(struct hz_bglobj **objlist, UNAT *objlist_count, struct hz_bglobj newobj)
{
	*objlist = oomrealloc(*objlist, ((*objlist_count) + 1) * sizeof(struct hz_bglobj));
	(*objlist)[*objlist_count] = newobj;
	(*objlist_count)++;
}