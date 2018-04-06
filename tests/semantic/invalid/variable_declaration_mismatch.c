/*
 * Multi-line comment.
 */

#include <stdint.h>
#include <stdio.h>

int64_t b[2];
b = 2;

/*
 * TODO find a way to trigger "buildMismatchTypeErrorMessage"
 * with "if (!recordedSymbol->getType()->equals(primaryType))"
 */
char v[2];
int32_t u;
u = v;