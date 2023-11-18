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
#include <string.h>
#include <stdio.h>
#include <ctype.h>

void nanojsonc_parse_array(const char *const json, NanoJSONcCallback callback, const char *const parentKey, void *object) {
    const char* error = NULL;
    if (json == NULL) return;
    const char *start = json, *cursor = NULL, *parent = parentKey == NULL ? "" : parentKey;
    int index = 0; // current index of the array

    for (cursor = start; *cursor != '\0' && *cursor != '['; cursor++); // begin bracket
    if (*cursor) cursor++; // proceed to values

    char key[NANOJSONC_KEY_SIZE]; // array subscript
    memset(key, 0, NANOJSONC_KEY_SIZE);

    while (*cursor != '\0') {

        if (isspace(*cursor) || *cursor == ',') { cursor++; continue; } // ignore whitespace

        // nested object
        if (*cursor == '{') { // begin brace
            start = cursor;
            for (int depth = 0; *cursor != '\0' && (depth > 1 || *cursor != '}'); cursor++) { // deep nested objects
                if (*cursor == '{') depth++;
                if (*cursor == '}') depth--;
            }
            cursor++; // include brace
            long len = cursor - start;

            char value[NANOJSONC_VALUE_SIZE];
            memset(value, 0, NANOJSONC_VALUE_SIZE);
            if (len >= NANOJSONC_VALUE_SIZE) {
                error = "Value exceeds buffer size";
                len = NANOJSONC_VALUE_SIZE - 1;
            }

            strncpy(value, start, len);
            value[len] = '\0';

            if (error) {
                if (callback(error, key, value, parent, object)) return;
                error = NULL;
            }

            char subKey[NANOJSONC_KEY_SIZE];
            memset(subKey, 0, NANOJSONC_KEY_SIZE);
            if (snprintf(subKey, sizeof(subKey), "%s[%d]", parent, index) < 0) { // parentKey with subscript
                if (callback("Formatted key exceeds buffer size", key, value, parent, object)) return;
            }

            nanojsonc_parse_object(value, callback, subKey, object);
            index++;
            cursor++;
        }

        // nested array
        if (*cursor == '[') { // begin bracket
            start = cursor;
            for (int depth = 0; *cursor != '\0' && (depth > 1 || *cursor != ']'); cursor++) { // deep nested arrays
                if (*cursor == '[') depth++;
                if (*cursor == ']') depth--;
            }
            cursor++; // include bracket
            long len = cursor - start;

            char value[NANOJSONC_VALUE_SIZE];
            memset(value, 0, NANOJSONC_VALUE_SIZE);
            if (len >= NANOJSONC_VALUE_SIZE) {
                error = "Value exceeds buffer size";
                len = NANOJSONC_VALUE_SIZE - 1;
            }

            strncpy(value, start, len);
            value[len] = '\0';

            if (error) {
                if(callback(error, key, value, parent, object)) return;
                error = NULL;
            }

            char subKey[NANOJSONC_KEY_SIZE];
            memset(subKey, 0, NANOJSONC_KEY_SIZE);
            if (snprintf(subKey, sizeof(subKey), "%s[%d]", parent, index) < 0) { // parentKey with subscript for the recursive call
                if(callback("Formatted key exceeds buffer size", key, value, parent, object)) return;
            }

            nanojsonc_parse_array(value, callback, subKey, object);
            index++;
            cursor++;
        }

        // parse values (string, number, boolean, null)
        if (snprintf(key, sizeof(key), "[%d]", index) < 0) {
            if(callback("Formatted key exceeds buffer size", key, NULL, parent, object)) return;
        }

        if (*cursor == '"') { // begin quote
            start = cursor + 1;
            for (cursor = start; *cursor != '\0' && *cursor != '\"'; cursor++); // end quote
            long len = cursor - start;

            char value[NANOJSONC_VALUE_SIZE];
            memset(value, 0, NANOJSONC_VALUE_SIZE);
            if (len >= NANOJSONC_VALUE_SIZE) {
                error = "Value exceeds buffer size";
                len = NANOJSONC_VALUE_SIZE - 1;
            }

            strncpy(value, start, len);
            value[len] = '\0';

            if (callback(error, key, value, parent, object)) return;

            error = NULL;
            index++;
            cursor++;
        }

        if (isdigit(*cursor)) { // value: number
            start = cursor;
            for (; *cursor != '\0' && isdigit(*cursor); cursor++); // end digit (non-whitespace)
            long len = cursor - start;

            char value[NANOJSONC_VALUE_SIZE];
            memset(value, 0, NANOJSONC_VALUE_SIZE);
            if (len >= NANOJSONC_VALUE_SIZE) {
                error = "Value exceeds buffer size";
                len = NANOJSONC_VALUE_SIZE - 1;
            }

            strncpy(value, start, len);
            value[len] = '\0';

            if (callback(error, key, value, parent, object)) return;
            
            error = NULL;
            index++;
            cursor++;
        }

        if (*cursor == 't' || *cursor == 'f' || *cursor == 'n') { // boolean (true/false) or null
            start = cursor;
            for (; *cursor != '\0' && !isspace(*cursor) && *cursor != ',' && *cursor != ']'; cursor++);
            long len = cursor - start;

            char value[NANOJSONC_VALUE_SIZE];
            memset(value, 0, NANOJSONC_VALUE_SIZE);
            if (len >= NANOJSONC_VALUE_SIZE) {
                error = "Value exceeds buffer size";
                len = NANOJSONC_VALUE_SIZE - 1;
            }

            strncpy(value, start, len);
            value[len] = '\0';

            if(callback(error, key, value, parent, object)) return;

            error = NULL;
            index++;
            cursor++;
        }

        if (*cursor == ']')
            cursor++;
    }
}
