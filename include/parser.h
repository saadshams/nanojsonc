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

/**
 * @file parser.h
 * @brief Header file for the nanoJSONc library.
 */

#ifndef NANOJSONC_PARSER_H
#define NANOJSONC_PARSER_H

#define KEY_SIZE	128
#define VALUE_SIZE	1024

/**
 * @brief Parse a JSON array and invoke a callback for each element.
 *
 * This function parses a JSON array and invokes the specified callback function
 * for each element found within the array. It supports the parsing of deep
 * nested objects and arrays and can handle various data types such as strings,
 * numbers, booleans, and null values.
 *
 * @param json The JSON array to parse.
 * @param callback The callback function to be invoked for each element.
 * @param parentKey The parent key (if applicable).
 * @param object A pointer to the object being populated or processed.
 *
 * The callback function should have the following signature:
 * @code
 * void callback(const char *const key, const char *const value,
 *               const char *const parentKey, void *object);
 * @endcode
 *
 * @note The provided callback function should handle the processing or population
 *       of the object based on the parsed JSON elements.
 *
 * @see nanojsonc_parse_object
 */
void nanojsonc_parse_array(const char *json, void (*callback)(const char *const key, const char *const value, const char *const parentKey, void *object), const char *parentKey, void *object);

/**
 * @brief Parse a JSON object and invoke a callback for each key-value pair.
 *
 * This function parses a JSON object and invokes the specified callback function
 * for each key-value pair within the object.
 *
 * @param json The JSON object to parse.
 * @param callback The callback function to be invoked for each key-value pair.
 * @param parentKey The parent key (if applicable).
 * @param object A pointer to the object being populated or processed.
 *
 * The callback function should have the following signature:
 * @code
 * void callback(const char *const key, const char *const value,
 *               const char *const parentKey, void *object);
 * @endcode
 *
 * @note The provided callback function should handle the processing or population
 *       of the object based on the parsed JSON key-value pairs.
 *
 * @see nanojsonc_parse_array
 */
void nanojsonc_parse_object(const char *json, void (*callback)(const char *const key, const char *const value, const char *const parentKey, void *object), const char *parentKey, void *object);

#endif //NANOJSONC_PARSER_H
