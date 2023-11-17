/**
BSD 3-Clause License

Copyright (c) 2023 Open Source Patterns Inc.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "parse.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

int counter = 0;

static void callback1(const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (strcmp(parentKey, "hobbies") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "Reading") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "Hiking") == 0); counter++; }
        if (strcmp(key, "[2]") == 0) { assert(strcmp(value, "Cooking") == 0); counter++; }
    }
}

static void testStringArray() {
    char *json = "[\"Reading\", \"Hiking\", \"Cooking\"]";
    nanojsonc_parse_array(json, callback1, "hobbies", NULL);
    assert(counter == 3);
    counter = 0;
}

static void callback2(const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (strcmp(parentKey, "numbers") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "1") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "22") == 0); counter++; }
        if (strcmp(key, "[2]") == 0) { assert(strcmp(value, "333") == 0); counter++; }
        if (strcmp(key, "[3]") == 0) { assert(strcmp(value, "4444") == 0); counter++; }
        if (strcmp(key, "[4]") == 0) { assert(strcmp(value, "55555") == 0); counter++; }
    }
}

static void testNumberArray() {
    char *json = "[1, 22, 333, 4444, 55555]";
    nanojsonc_parse_array(json, callback2, "numbers", NULL);
    assert(counter == 5);
    counter = 0;
}

static void callback3(const char *const key, const char *const value, const char *const parentKey, void *object) {
    counter++;
}

static void testEmptyArray() {
    nanojsonc_parse_array("", callback3, "empty", NULL);
     assert(counter == 0);
    nanojsonc_parse_array("[]", callback3, "empty", NULL);
    assert(counter == 0);
    nanojsonc_parse_array(NULL, callback3, "empty", NULL);
    assert(counter == 0);
    nanojsonc_parse_array(NULL, callback3, NULL, NULL);
    assert(counter == 0);
}

static void callback4(const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (strcmp(parentKey, "single") == 0) {
        if(strcmp(key, "[0]") == 0) { assert(strcmp(value, "2023") == 0); counter++; }
    }
}

static void testSingleItemArray() {
    char *json = "[2023]";
    nanojsonc_parse_array(json, callback4, "single", NULL);
    assert(counter == 1);
    counter = 0;
}

static void callback5(const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (strcmp(parentKey, "boolean") == 0) {
        if(strcmp(key, "[0]") == 0) { assert(strcmp(value, "true") == 0); counter++; }
        if(strcmp(key, "[1]") == 0) { assert(strcmp(value, "false") == 0); counter++; }
        if(strcmp(key, "[2]") == 0) { assert(strcmp(value, "true") == 0); counter++; }
    }
}

static void testBooleanArray() {
    char *json = "[true, false, true]";
    nanojsonc_parse_array(json, callback5, "boolean", NULL);
    assert(counter == 3);
    counter = 0;
}

static void callback6(const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (strcmp(parentKey, "Null") == 0) {
        if(strcmp(key, "[0]") == 0) { assert(strcmp(value, "null") == 0); counter++; }
        if(strcmp(key, "[1]") == 0) { assert(strcmp(value, "null") == 0); counter++; }
        if(strcmp(key, "[2]") == 0) { assert(strcmp(value, "null") == 0); counter++; }
    }
}

static void testNullArray() {
    char *json = "[null, null, null]";
    nanojsonc_parse_array(json, callback6, "Null", NULL);
    assert(counter == 3);
    counter = 0;
}

static void callback7(const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (strcmp(parentKey, "single[0]") == 0) {
        if(strcmp(key, "i") == 0) { assert(strcmp(value, "4") == 0); counter++; }
    }
}

static void testSingleObjectArray() {
    char *json = "[{\"i\": 4 }]";
    nanojsonc_parse_array(json, callback7, "single", NULL);
    assert(counter == 1);
    counter = 0;
}

static void callback8(const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (strcmp(parentKey, "multiple[0]") == 0) {
        if (strcmp(key, "a") == 0) { assert(strcmp(value, "1") == 0); counter++; }
        if (strcmp(key, "b") == 0) { assert(strcmp(value, "2") == 0); counter++; }
    }
    if (strcmp(parentKey, "multiple[1]") == 0) {
        if (strcmp(key, "a") == 0) { assert(strcmp(value, "3") == 0); counter++; }
        if (strcmp(key, "b") == 0) { assert(strcmp(value, "4") == 0); counter++; }
    }
}

static void testMultipleObjectsArray() {
    char *json = "[{\"a\":1,\"b\":2}, {\"a\":3,\"b\":4}]";
    nanojsonc_parse_array(json, callback8, "multiple", NULL);
    assert(counter == 4);
    counter = 0;
}

static void callback9(const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (strcmp(parentKey, "boolean[0]") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "true") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "false") == 0); counter++; }
        if (strcmp(key, "[2]") == 0) { assert(strcmp(value, "true") == 0); counter++; }
    }
    if (strcmp(parentKey, "boolean[1]") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "false") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "false") == 0); counter++; }
    }
}

static void testNestedArray() { // has to be parsed list, not whole values
    char *json = "[[true, false, true], [false, false]]";
    nanojsonc_parse_array(json, callback9, "boolean", NULL);
    assert(counter == 5);
    counter = 0;
}

static void callback10(const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (strcmp(parentKey, "mixed") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "Reading") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "2020") == 0); counter++; }
        if (strcmp(key, "[2]") == 0) { assert(strcmp(value, "null") == 0); counter++; }
        if (strcmp(key, "[3]") == 0) { assert(strcmp(value, "true") == 0); counter++; }
        if (strcmp(key, "[4]") == 0) { assert(strcmp(value, "false") == 0); counter++; }
    } else if (strcmp(parentKey, "mixed[5]") == 0) {
        if (strcmp(key, "a") == 0) { assert(strcmp(value, "1") == 0); counter++; }
        if (strcmp(key, "b") == 0) { assert(strcmp(value, "2") == 0); counter++; }
    }
}

static void testMixedArray() {
    char *json = "[\"Reading\", 2020, null, true, false, {\"a\": 1, \"b\": 2}]";
    nanojsonc_parse_array(json, callback10, "mixed", NULL);
    assert(counter == 7);
    counter = 0;
}

static void callback11(const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (strcmp(parentKey, "data[0][name]") == 0) {
        if (strcmp(key, "first") == 0) { assert(strcmp(value, "Janie") == 0); counter++; }
        if (strcmp(key, "last") == 0) { assert(strcmp(value, "Doe") == 0); counter++; }
    }
    if (strcmp(parentKey, "data[0]") == 0) {
        if (strcmp(key, "age") == 0) { assert(atoi(value) == 13); counter++; }
    }
}

static void testNestedObject() {
    char *json = "[{\"name\": {\"first\": \"Janie\",\"last\": \"Doe\"},\"age\": 13}]";
    nanojsonc_parse_array(json, callback11, "data", NULL);
    assert(counter == 3);
    counter = 0;
}

static void callback12(const char *const key, const char *const value, const char *const parentKey, void *object) {
    if (strcmp(parentKey, "data") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "k") == 0); counter++; }
    } else if (strcmp(parentKey, "data[1]") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "r") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "h") == 0); counter++; }
        if (strcmp(key, "[2]") == 0) { assert(strcmp(value, "c") == 0); counter++; }
    } else if (strcmp(parentKey, "data[2]") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "a") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "b") == 0); counter++; }
    } else if (strcmp(parentKey, "data[2][2]") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "x") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "y") == 0); counter++; }
        if (strcmp(key, "[2]") == 0) { assert(strcmp(value, "z") == 0); counter++; }
    } else if (strcmp(parentKey, "data[2][2][3]") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "j") == 0); counter++; }
    }
}

static void testDeepNestedArray() {
    char *json = "[\"k\", [\"r\", \"h\", \"c\"], [\"a\", \"b\", [\"x\", \"y\", \"z\", [\"j\"]]]]]";
    nanojsonc_parse_array(json, callback12, "data", NULL);
    assert(counter == 10);
    counter = 0;
}

int main() {
    // flat arrays
    testStringArray();
    testNumberArray();
    testEmptyArray();
    testSingleItemArray();
    testBooleanArray();
    testNullArray();

    // nested arrays
    testSingleObjectArray();
    testMultipleObjectsArray();
    testNestedArray();
    testMixedArray();
    testNestedObject();
    testDeepNestedArray();

    return 0;
}
