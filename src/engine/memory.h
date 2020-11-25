#ifndef HZMEM_HEADER_FILE
#define HZMEM_HEADER_FILE
#include "engine.h"
/* Malloc/realloc which reports memory errors and exits. */
H_EXTERN X0 *oomrealloc(X0 *ptr, SX size);
#endif