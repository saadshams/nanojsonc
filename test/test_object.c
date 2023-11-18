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

#include "parser.h"
#include <assert.h>
#include <string.h>

int counter = 0;

static int callback(const char *const error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    counter++;
    return 0;
}

static void testEmptyObject() {
    nanojsonc_parse_object("", callback, NULL, NULL);
    assert(counter == 0);
    nanojsonc_parse_object("{}", callback, NULL, NULL);
    assert(counter == 0);
    nanojsonc_parse_object(NULL, callback, "empty", NULL);
    assert(counter == 0);
    nanojsonc_parse_object(NULL, callback, NULL, NULL);
    assert(counter == 0);
}

static int callback1(const char *const error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(key, "first") == 0) { assert(strcmp(value, "John") == 0); counter++; }
    if (strcmp(key, "last") == 0) { assert(strcmp(value, "Doe") == 0); counter++; }
    if (strcmp(key, "phone") == 0) { assert(strcmp(value, "") == 0); counter++; }
    return 0;
}

static void testStringProperty() {
    char *json = "{\"first\": \"John\", \"last\": \"Doe\", \"phone\": \"\"}";
    nanojsonc_parse_object(json, callback1, NULL, NULL);
    assert(counter == 3);
    counter = 0;
}

static int callback2(const char *const error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(key, "first") == 0) { assert(strcmp(value, "John") == 0); counter++; }
    if (strcmp(key, "last") == 0) { assert(strcmp(value, "Doe") == 0); counter++; }
    if (strcmp(key, "age") == 0) { assert(strcmp(value, "15") == 0); counter++; }
    return 0;
}

static void testNumberProperty() {
    char *json = "{\"first\": \"John\", \"last\": \"Doe\", \"age\": 15}";
    nanojsonc_parse_object(json, callback2, NULL, NULL);
    assert(counter == 3);
    counter = 0;
}

static int callback3(const char *const error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(key, "first") == 0) { assert(strcmp(value, "John") == 0); counter++; }
    if (strcmp(key, "last") == 0) { assert(strcmp(value, "Doe") == 0); counter++; }
    if (strcmp(key, "isMarried") == 0) { assert(strcmp(value, "false") == 0); counter++; }
    if (strcmp(key, "isEmployed") == 0) { assert(strcmp(value, "true") == 0); counter++; }
    return 0;
}

static void testBooleanProperty() {
    char *json = "{\"first\": \"John\", \"last\": \"Doe\", \"isMarried\": false, \"isEmployed\": true}";
    nanojsonc_parse_object(json, callback3, NULL, NULL);
    assert(counter == 4);
    counter = 0;
}

static int callback4(const char *const error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(key, "first") == 0) { assert(strcmp(value, "John") == 0); counter++; }
    if (strcmp(key, "last") == 0) { assert(strcmp(value, "Doe") == 0); counter++; }
    if (strcmp(key, "phone") == 0) { assert(strcmp(value, "null") == 0); counter++; }
    return 0;
}

static void testNullValue() {
    char *json = "{\"first\": \"John\", \"last\": \"Doe\", \"phone\": null}";
    nanojsonc_parse_object(json, callback4, NULL, NULL);
    assert(counter == 3);
    counter = 0;
}

static int callback5(const char *const error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(parentKey, "") == 0) {
        if (strcmp(key, "first") == 0) { assert(strcmp(value, "John") == 0); counter++; }
        if (strcmp(key, "age") == 0) { assert(strcmp(value, "15") == 0); counter++; }
    }
    if (strcmp(parentKey, "[hobbies]") == 0) {
        if (strcmp(key, "[0]") == 0) { assert(strcmp(value, "Reading") == 0); counter++; }
        if (strcmp(key, "[1]") == 0) { assert(strcmp(value, "Hiking") == 0); counter++; }
        if (strcmp(key, "[2]") == 0) { assert(strcmp(value, "Cooking") == 0); counter++; }
    }
    return 0;
}

static void testArrayProperty() {
    char *json = "{\"first\": \"John\", \"hobbies\": [\"Reading\", \"Hiking\", \"Cooking\"], \"age\": 15}";
    nanojsonc_parse_object(json, callback5, NULL, NULL);
    assert(counter == 5);
    counter = 0;
}

static int callback6(const char *const error, const char *const key, const char *const value, const char *const parentKey, void *object) {
    assert(!error);
    if (strcmp(parentKey, "") == 0) {
        if (strcmp(key, "foo") == 0) { assert(strcmp(value, "bar") == 0); counter++; }
    }
    if (strcmp(parentKey, "[level1]") == 0) {
        assert(strcmp(key, "property1") == 0);
        assert(strcmp(value, "value1") == 0);
        counter++;
    } else if (strcmp(parentKey, "[level1][level2]") == 0) {
        assert(strcmp(key, "property2") == 0);
        assert(strcmp(value, "value2") == 0);
        counter++;
    } else if (strcmp(parentKey, "[level1][level2][level3]") == 0) {
        assert(strcmp(key, "property3") == 0);
        assert(strcmp(value, "value3") == 0);
        counter++;
    }
    return 0;
}

static void testDeepNested() {
    char *json = "{\"foo\": \"bar\", \"level1\": {\"property1\": \"value1\", \"level2\": {\"property2\": \"value2\", \"level3\": {\"property3\": \"value3\"}}}}";
    nanojsonc_parse_object(json, callback6, NULL, NULL);
    assert(counter == 4);
    counter = 0;
}

int main() {
    testEmptyObject();
    testStringProperty();
    testNumberProperty();
    testBooleanProperty();
    testNullValue();
    testArrayProperty();

    testDeepNested();

    return 0;
}
