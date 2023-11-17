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

void nanojsonc_parse_object(const char *const json, void (*callback)(const char *const key, const char *const value, const char *const parentKey, void *object), const char *const parentKey, void *object) {
    if (json == NULL) return;
    const char *start = json, *parent = (parentKey == NULL) ? "" : parentKey, *cursor = NULL;

    for (cursor = start; *cursor != '\0' && *cursor != '{'; cursor++); // begin brace
    if (*cursor) cursor++; // proceed to key-value pairs

    while (*cursor) {
        for (; *cursor != '\0' && isspace((unsigned char) *cursor); cursor++); // whitespace

        // parse key
        for (; *cursor != '\0' && *cursor != '\"'; cursor++); // key: begin quote
        if (*cursor == '\0') return; // empty or no key-value pairs

        start = cursor + 1;
        for (cursor = start; *cursor != '\0' && *cursor != '\"'; cursor++); // key: end quote
        long length = cursor - start;

        char key[KEY_SIZE];
        memset(key, 0, KEY_SIZE);
        strncpy(key, start, length);
        key[length] = '\0';
        cursor++;

        // skip whitespace after the key
        for (; *cursor != '\0' && isspace((unsigned char) *cursor); cursor++);

        if (*cursor) cursor++; // skip colon

        // skip whitespace before the value
        for (; *cursor != '\0' && isspace((unsigned char) *cursor); cursor++);

        // nested object
        if (*cursor == '{') { // begin brace
            start = cursor;
            for (int depth = 0; *cursor != '\0' && (depth > 1 || *cursor != '}'); cursor++) { // deep nested objects
                if (*cursor == '{') depth++; // causes to skip
                if (*cursor == '}') depth--;
            }
            cursor++; // include brace

            size_t len = cursor - start;
            char value[VALUE_SIZE];
            memset(value, 0, VALUE_SIZE);
            strncpy(value, start, len);
            value[len] = '\0';

            char subKey[KEY_SIZE];
            memset(subKey, 0, KEY_SIZE);
            if (snprintf(subKey, KEY_SIZE, "%s[%s]", parent, key) < 0) // parentKey with childKey
                perror("Formatted key exceeds buffer size");

            nanojsonc_parse_object(value, callback, subKey, object);
        }

        // nested array
        if (*cursor == '[') { // value: array
            start = cursor;
            for (int depth = 0; *cursor != '\0' && (depth > 1 || *cursor != ']'); cursor++) { // deep nested arrays
                if (*cursor == '[') depth++;
                if (*cursor == ']') depth--;
            }
            cursor++; // include bracket
            long len = cursor - start;

            char value[VALUE_SIZE];
            memset(value, 0, VALUE_SIZE);
            strncpy(value, start, len);
            value[len] = '\0';

            char subKey[KEY_SIZE];
            memset(subKey, 0, KEY_SIZE);
            if (snprintf(subKey, sizeof(subKey), "%s[%s]", parent, key) < 0) // parentKey with childKey
                perror("Formatted key exceeds buffer size");

            nanojsonc_parse_array(value, callback, subKey, object);
        }

        // parse values (string, number, boolean, null)

        if (*cursor == '"') { // value: string
            start = cursor + 1;
            for (cursor = start; *cursor != '\0' && *cursor != '\"'; cursor++); // end quote
            long len = cursor - start;

            char value[VALUE_SIZE];
            memset(value, 0, VALUE_SIZE);
            strncpy(value, start, len);
            value[len] = '\0';

            callback(key, value, parent, object);
            cursor++;
        }

        if (isdigit(*cursor)) { // value: number
            start = cursor;
            for (; *cursor != '\0' && isdigit(*cursor); cursor++); // end digit (non-whitespace)
            long len = cursor - start;

            char value[VALUE_SIZE];
            memset(value, 0, KEY_SIZE);
            strncpy(value, start, len);
            value[len] = '\0';
            callback(key, value, parent, object);
        }

        if (*cursor == 't' || *cursor == 'f' || *cursor == 'n') { // boolean (true/false) or null
            start = cursor;
            for (; *cursor != '\0' && !isspace(*cursor) && *cursor != ',' && *cursor != '}'; cursor++);
            long len = cursor - start;

            char value[VALUE_SIZE];
            memset(value, 0, VALUE_SIZE);
            strncpy(value, start, len);
            value[len] = '\0';

            callback(key, value, parent, object);
        }

        // skip whitespace after the value
        for (; *cursor != '\0' && isspace(*cursor); cursor++);

        if (*cursor) cursor++; // either EOF or comma
    }
}
