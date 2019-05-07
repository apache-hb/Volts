#pragma once

// for concatenating tokens at compile time
// eg CAT(__, AA) expands into __AA
// this also works for macros such as __LINE__ 
// eg CAT(_L_, __LINE) expands into _L_6
#define CAT_INNER(A, B) A##B
#define CAT(A, B) CAT_INNER(A, B)
