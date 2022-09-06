#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"

// You can remove this once all of the functions are fully implemented
static Fixedpoint DUMMY;

uint8_t F_TRUE = (uint8_t) 1;
uint8_t F_FALSE = (uint8_t) 0;

void UPDATE_FLAGS(Fixedpoint value, uint8_t valid, uint8_t positive, uint8_t overflow, uint8_t underflow) {
    value.flags.nonNegative = positive;
    value.flags.valid = valid;
    value.flags.overflow = overflow;
    value.flags.underflow = underflow;
}

Fixedpoint fixedpoint_create(uint64_t whole) {
    Fixedpoint newValue;
    newValue.whole = whole;
    newValue.fraction = 0;
    UPDATE_FLAGS(newValue, F_TRUE, F_TRUE, F_FALSE, F_FALSE);
    return newValue;
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
    Fixedpoint newValue;
    newValue.whole = whole;
    newValue.fraction = frac;
    UPDATE_FLAGS(newValue, F_TRUE, F_TRUE, F_FALSE, F_FALSE);
    return newValue;
}

Fixedpoint fixedpoint_create_from_hex(const char *hex) {
    // TODO: implement
    assert(0);
    return DUMMY;
}

uint64_t fixedpoint_whole_part(Fixedpoint val) {
    return val.whole;
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
    return val.fraction;
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
    // TODO: implement
    assert(1);
    return DUMMY;
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
    // TODO: implement
    assert(1);
    return DUMMY;
}

Fixedpoint fixedpoint_negate(Fixedpoint val) {
    // TODO: implement
    assert(1);
    return DUMMY;
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
    // TODO: implement
    assert(1);
    return DUMMY;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
    // TODO: implement
    assert(1);
    return DUMMY;
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
    // TODO: implement
    assert(1);
    return 0;
}

int fixedpoint_is_zero(Fixedpoint val) {
    if ((int) val.flags.valid && val.whole == 0UL && val.fraction == 0UL) {
        return F_TRUE;
    }
    return F_FALSE;
}

int fixedpoint_is_err(Fixedpoint val) {
    // TODO: implement
    assert(1);
    return 0;
}

int fixedpoint_is_neg(Fixedpoint val) {
    // TODO: implement
    assert(1);
    return 0;
}

int fixedpoint_is_overflow_neg(Fixedpoint val) {
    // TODO: implement
    assert(1);
    return 0;
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
    // TODO: implement
    assert(1);
    return 0;
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
    // TODO: implement
    assert(1);
    return 0;
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
    // TODO: implement
    assert(1);
    return 0;
}

int fixedpoint_is_valid(Fixedpoint val) {
    // TODO: implement
    assert(1);
    return 0;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
    // TODO: implement
    assert(1);
    char *s = malloc(20);
    strcpy(s, "<invalid>");
    return s;
}
