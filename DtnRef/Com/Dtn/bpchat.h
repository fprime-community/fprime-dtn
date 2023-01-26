#ifndef BPCHAT_H
#define BPCHAT_H

// Taken from `ion-core/Makefile`
#define DSPACE_ORDER 3
#define BP_EXTENDED
#include <pthread.h>  // TODO needed because some ION preprocessor logic is broken
#include <sys/time.h> // TODO needed because some ION preprocessor logic is broken
#include "bp.h"

int start(int argc, char **argv);

#endif // BPCHAT_H
