#ifndef HZGLO_HEADER_FILE
#define HZGLO_HEADER_FILE
#include "../shared/hzshared.h"
struct hz_bglobj {
	U64 namehash;
	union {
		UNAT u;
		INAT i;
	} glid;
	U8 vtype;
	U1 failed;
};

H_EXTERN INAT bglobjsrch(struct hz_bglobj *o, INAT l, INAT r, U64 x);
H_EXTERN struct hz_bglobj get_bglobj(struct hz_bglobj *objlist, UNAT objlist_count, U64 snmid);
H_EXTERN X0 sort_bglobj_list(struct hz_bglobj *objlist, UNAT objlist_count);
#endif
