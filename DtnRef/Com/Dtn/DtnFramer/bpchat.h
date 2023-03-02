#ifndef BPCHAT_H
#define BPCHAT_H

// Taken from `ion-core/Makefile`
#define DSPACE_ORDER 3
#define BP_EXTENDED
#include <pthread.h>  // TODO needed because some ION preprocessor logic is broken
#include <sys/time.h> // TODO needed because some ION preprocessor logic is broken
#include "bp.h"

//! @brief Start bpchat
//! @param _ownEid own EID, intentionally mutable to appease ION
//! @param _destEid destination EID, intentionally mutable to appease ION
int bpchat_start(char *_ownEid, char *_destEid);

#endif // BPCHAT_H
