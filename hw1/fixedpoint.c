#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "fixedpoint.h"

// You can remove this once all of the functions are fully implemented
static Fixedpoint DUMMY;

Fixedpoint fixedpoint_create(uint64_t whole) {
    return fixedpoint_create2(whole,0);
}

Fixedpoint fixedpoint_create2(uint64_t whole, uint64_t frac) {
    Fixedpoint fp = {
            .whole_p = whole,
            .frac_p = frac,
            .tag = VALID_NONNEG,
    };
    return fp;
}

Fixedpoint fixedpoint_create_from_hex(const char *hex) {
    // create copy string
    char *copy = malloc(strlen(hex) + 1);
    strcpy(copy, hex);

    // copy whole and frac values to new strings
    Fixedpoint fp = {
            .whole_p = 0UL,
            .frac_p = 0UL,
            .tag = VALID_NONNEG,
    };
    char *wholeS = NULL;
    char *fracS = NULL;
    char *ptr = strtok(copy, ".");

    if (copy[0] == '-') {
        fp.tag = VALID_NEG;
        ptr++;
    }

    if (copy[0] == '.') {
        wholeS = "0";
        fracS = ptr;
        size_t extra = 16 - strlen(fracS);
        char *zeroes = "0000000000000000";
        strncat(fracS, zeroes, extra);
        ptr = strtok(NULL, ".");
        if (ptr) {
            fp.tag = ERROR_VALUE;
            fp.frac_p = 0UL;
            fp.whole_p = 0UL;
            return fp;
        }
    } else {
        if (ptr) {
            wholeS = ptr;
        } else {
            wholeS = "0";
        }

        ptr = strtok(NULL, ".");
        if (ptr) {
            // add zeroes to frac string
            fracS = ptr;
            size_t extra = 16 - strlen(fracS);
            char *zeroes = "0000000000000000";
            strncat(fracS, zeroes, extra);
        } else {
            fracS = "0000000000000000";
        }
    }

    // check if whole and frac strings are valid
    char *temp = wholeS;
    while (temp != NULL && *temp != '\0') {
        if ((*temp >= '0' && *temp <= '9') || (*temp >= 'A' && *temp <= 'F')
        || (*temp >= 'a' && *temp <= 'f')) {
            temp++;
            continue;
        } else {
            fp.tag = ERROR_VALUE;
            fp.frac_p = 0UL;
            fp.whole_p = 0UL;
            return fp;
        }
    }

    fp.whole_p = strtoull(wholeS, NULL, 16);
    fp.frac_p = 0UL;
    if (fracS != NULL) {
        fp.frac_p = strtoull(fracS, NULL, 16);
    }

    free(copy);
    return fp;
}

uint64_t fixedpoint_whole_part(Fixedpoint val) {
    assert(fixedpoint_is_valid(val));
    return val.whole_p;
}

uint64_t fixedpoint_frac_part(Fixedpoint val) {
    assert(fixedpoint_is_valid(val));
    return val.frac_p;
}

Fixedpoint fixedpoint_add(Fixedpoint left, Fixedpoint right) {
    assert(fixedpoint_is_valid(left));
    assert(fixedpoint_is_valid(right));

    Fixedpoint fp2;
    int leftIsGreater = 2;

    // DIFFERENT SIGNS

    // assign correct tag
    if (left.tag == VALID_NONNEG && right.tag == VALID_NEG) {
        leftIsGreater = fixedpoint_compare(left, fixedpoint_negate(right));
        if (leftIsGreater == 1) {
            fp2.tag = VALID_NONNEG;
        } else if (leftIsGreater == -1) {
            fp2.tag = VALID_NEG;
        }
    }
    if (left.tag == VALID_NEG && right.tag == VALID_NONNEG) {
        leftIsGreater = fixedpoint_compare(fixedpoint_negate(left), right);
        if (leftIsGreater == 1) {
            fp2.tag = VALID_NEG;
        } else if (leftIsGreater == -1) {
            fp2.tag = VALID_NONNEG;
        }
    }
    if (leftIsGreater == 0) {
        return fixedpoint_create(0UL);
    }

    // calculate value
    if (leftIsGreater == 1) {
        fp2.whole_p = left.whole_p - right.whole_p;
        fp2.frac_p = left.frac_p - right.frac_p;
        // accomodate underflow
        if (fp2.frac_p > left.frac_p) {
            uint64_t leftFracAvailable = 0xFFFFFFFFFFFFFFFFUL - left.frac_p;
            fp2.frac_p =  right.frac_p - leftFracAvailable;
            fp2.whole_p -= 1UL;
        }
    } else if (leftIsGreater == -1) {
        fp2.whole_p = right.whole_p - left.whole_p;
        fp2.frac_p = right.frac_p - left.frac_p;
        // accomodate underflow
        if (fp2.frac_p > right.frac_p) {
            uint64_t rightFracAvailable = 0xFFFFFFFFFFFFFFFFUL - right.frac_p;
            fp2.frac_p =  left.frac_p - rightFracAvailable;
            fp2.whole_p -= 1UL;
        }
    }

    // return value
    if ((left.tag == VALID_NONNEG && right.tag == VALID_NEG) || (left.tag == VALID_NEG && right.tag == VALID_NONNEG)) {
        return fp2;
    }


    // SAME SIGN
    fp2.whole_p = left.whole_p + right.whole_p;
    fp2.frac_p = left.frac_p + right.frac_p;
    // Handle fraction part overflow
    if (fp2.frac_p < left.frac_p) {
        // update fp2.frac_p to overflow value
        uint64_t leftFracAvailable = 0xFFFFFFFFFFFFFFFFUL - left.frac_p;
        fp2.frac_p =  right.frac_p - leftFracAvailable;
        fp2.whole_p += 1UL;
    }

    // invalid values
    if (left.tag == VALID_NONNEG && right.tag == VALID_NONNEG) {
        fp2.tag = VALID_NONNEG;
        if (fp2.whole_p < left.whole_p) {
            fp2.tag = POS_OVERFLOW;
        }
    }
    if (left.tag == VALID_NEG && right.tag == VALID_NEG) {
        fp2.tag = VALID_NEG;
        if (fp2.whole_p < left.whole_p) {
            fp2.tag = NEG_OVERFLOW;
        }
    }
    return fp2;
}

Fixedpoint fixedpoint_sub(Fixedpoint left, Fixedpoint right) {
    return fixedpoint_add(left, fixedpoint_negate(right));
}

Fixedpoint fixedpoint_negate(Fixedpoint val) {
    assert (fixedpoint_is_valid(val));

    if (val.tag == VALID_NONNEG) {
        val.tag = VALID_NEG;
    }
    if (val.tag == VALID_NEG) {
        val.tag = VALID_NONNEG;
    }

    return val;
}

Fixedpoint fixedpoint_halve(Fixedpoint val) {
    // TODO: implement
    assert(fixedpoint_is_valid(val));
    Fixedpoint fp2;
    fp2.whole_p = val.whole_p/2;
    fp2.frac_p = val.frac_p/2;
    fp2.tag = val.tag;
    // check underflow
    return fp2;
}

Fixedpoint fixedpoint_double(Fixedpoint val) {
    assert (fixedpoint_is_valid(val));
    Fixedpoint fp2;
    fp2 = fixedpoint_add(val, val);
    return fp2;
}

int fixedpoint_compare(Fixedpoint left, Fixedpoint right) {
    // different signs
    if (left.tag == VALID_NONNEG && right.tag == VALID_NEG) {
        return 1;
    }
    if (right.tag == VALID_NONNEG && left.tag == VALID_NEG) {
        return -1;
    }

    // same sign
    int temp;
    if (left.whole_p > right.whole_p) {
        temp = 1;
    } else if (left.whole_p < right.whole_p) {
        temp = -1;
    } else if (left.frac_p > right.frac_p) {
        temp = 1;
    } else if (left.frac_p < right.frac_p) {
        temp = -1;
    } else {
        temp = 0;
    }

    // if both signs are negative, return opposite value
    if (left.tag == VALID_NONNEG) {
        return temp;
    }
    return -temp;
}

int fixedpoint_is_zero(Fixedpoint val) {
    assert(fixedpoint_is_valid(val));
    if (val.whole_p == 0 && val.frac_p == 0) {
        return 1;
    }
    return 0;
}

int fixedpoint_is_err(Fixedpoint val) {
    return val.tag == ERROR_VALUE;
}

int fixedpoint_is_neg(Fixedpoint val) {
    assert(fixedpoint_is_valid(val));
    return val.tag == VALID_NEG;
}

int fixedpoint_is_overflow_neg(Fixedpoint val) {
    return val.tag == NEG_OVERFLOW;
}

int fixedpoint_is_overflow_pos(Fixedpoint val) {
    return val.tag == POS_OVERFLOW;
}

int fixedpoint_is_underflow_neg(Fixedpoint val) {
    return val.tag == NEG_UNDERFLOW;
}

int fixedpoint_is_underflow_pos(Fixedpoint val) {
    return val.tag == POS_UNDERFLOW;
}

int fixedpoint_is_valid(Fixedpoint val) {
    return val.tag == VALID_NEG || val.tag == VALID_NONNEG;
}

char *fixedpoint_format_as_hex(Fixedpoint val) {
    // TODO: implement
    assert(0);
    assert(fixedpoint_is_valid(val));
    char *s = malloc(34);

    sprintf(s, "%llx", val.whole_p);
    if (val.frac_p != 0UL) {
        char *fracStr = malloc(17);
        sprintf(fracStr, ".%llx", val.frac_p);
        char *ptr = fracStr;
        while (ptr != NULL || ptr != '0') {
            ptr++;
        }
        ptr = '\0';
        strncat(s, fracStr, strlen(fracStr));
        free(fracStr);
    }

    if (val.tag == VALID_NEG) {
        char *temp = "-";
        strncat(temp, s, strlen(s));
        strcpy(s, temp);
    }
    return s;
}
