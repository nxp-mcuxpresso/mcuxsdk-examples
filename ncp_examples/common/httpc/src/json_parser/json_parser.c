/** Copyright (c) 2010 Serge A. Zaitsev
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * Copyright 2008-2020 NXP
 */

/*
 *  Derived from:
 *  http://zserge.com/jsmn.html
 */
#include <stdlib.h>
#include <wmerrno.h>
#include <wmtypes.h>
#include <string.h>
#include <json_parser.h>
#include <jsmn.h>
#include <osa.h>
#include <wm_utils.h>
#define CONFIG_JSMN_PARENT_LINKS
#define CONFIG_JSMN_STRICT
#ifndef CONFIG_JSMN_PARENT_LINKS
#error JSON Parser requires CONFIG_JSMN_PARENT_LINKS
#endif

#ifndef CONFIG_JSMN_STRICT
#error JSON Parser requires CONFIG_JSMN_STRICT
#endif

/* Returns true if an exact string match is found, else false */
static bool json_token_streq(char *js, jsontok_t *t, char *s)
{
    return (strncmp(js + t->start, s, t->end - t->start) == 0 && strlen(s) == (size_t)(t->end - t->start));
}

/* Skips to the last element of an array or object.
 * If there is an array/object inside the given array/object,
 * the function is called recursively to skip all elements
 */
static jsontok_t *skip_to_last(jsontok_t *element)
{
    jsontok_t *t = element;
    if (t->size == 0)
        return t;
    int cnt = t->size;
    while (cnt--)
    {
        t++;
        if (t->size)
            t = skip_to_last(t);
    }
    return t;
}
/* Converts the value held by the token into a boolean */
static int json_str_to_bool(jobj_t *jobj, jsontok_t *t, bool *value)
{
    if (!t || t->type != JSMN_PRIMITIVE)
        return -WM_E_JSON_INVALID_TYPE;
    if (json_token_streq(jobj->js, t, "true") || json_token_streq(jobj->js, t, "1"))
        *value = true;
    else if (json_token_streq(jobj->js, t, "false") || json_token_streq(jobj->js, t, "0"))
        *value = false;
    else
        return -WM_E_JSON_INVALID_TYPE;
    return WM_SUCCESS;
}

/* Converts the value held by the token into an integer */
static int json_str_to_int(jobj_t *jobj, jsontok_t *t, int *value)
{
    if (!t || t->type != JSMN_PRIMITIVE)
        return -WM_E_JSON_INVALID_TYPE;
    char *endptr;
    int int_val = strtoul(&jobj->js[t->start], &endptr, 10);
    if (endptr == &(jobj->js[t->end]))
    {
        *value = int_val;
        return WM_SUCCESS;
    }
    else
    {
        return -WM_E_JSON_INVALID_TYPE;
    }
}
/* Converts the value held by the token into an int64 */
static int json_str_to_int64(jobj_t *jobj, jsontok_t *t, int64_t *value)
{
    if (!t || t->type != JSMN_PRIMITIVE)
        return -WM_E_JSON_INVALID_TYPE;
    char *endptr;
    int64_t int_val = strtoull(&jobj->js[t->start], &endptr, 10);
    if (endptr == &(jobj->js[t->end]))
    {
        *value = int_val;
        return WM_SUCCESS;
    }
    else
    {
        return -WM_E_JSON_INVALID_TYPE;
    }
}

/* Converts the value held by the token into a float */
static int json_str_to_float(jobj_t *jobj, jsontok_t *t, float *value)
{
    if (!t || t->type != JSMN_PRIMITIVE)
        return -WM_E_JSON_INVALID_TYPE;
    char *start_ptr = &jobj->js[t->start];
    char *endptr;

    *value = wm_strtof(start_ptr, &endptr);
    if (endptr != &(jobj->js[t->end]))
        return -WM_E_JSON_INVALID_TYPE;
    return WM_SUCCESS;
}

/* Converts the value held by the token into a null terminated string */
static int json_str_to_str(jobj_t *jobj, jsontok_t *t, char *value, int maxlen)
{
    if (!t || t->type != JSMN_STRING)
        return -WM_E_JSON_INVALID_TYPE;
    if ((t->end - t->start) >= maxlen)
        return -WM_E_JSON_NOMEM;
    strncpy(value, jobj->js + t->start, t->end - t->start);
    value[t->end - t->start] = 0;
    return WM_SUCCESS;
}

/* Searches for json element inside an object based on the key and populates
 * the val_t token if element is found and returns WM_SUCCESS.
 * If not found, returns error.
 */
static int json_get_value(jobj_t *jobj, char *key, jsontok_t **val_t)
{
    jsontok_t *t     = jobj->cur;
    int num_children = t->size;
    *val_t           = NULL;
    /* If there are no children it is an error, since we
     * would not find the key at all
     */
    if (num_children == 0)
        return -WM_E_JSON_FAIL;

    /* If the current token type is not an object, it is an error */
    if (t->type != JSMN_OBJECT)
        return -WM_E_JSON_INVALID_JOBJ;
    while (num_children--)
    {
        /* Increment the token pointer first so that we begin from the
         * first token inside the object.
         */
        t++;
        /* For safety, check if the current token's end does not go
         * beyond the parent object's end. This case is unlikely, yet,
         * better to have a check.
         */
        if (t->end > jobj->cur->end)
            return -WM_E_JSON_FAIL;
        /* First token inside an object should be a key.
         * If not, it is an error.
         */
        if (t->type != JSMN_STRING)
            return -WM_E_JSON_FAIL;
        /* If the key matches with the given key, the member
         * has been found.
         * Else, just skip the value.
         */
        if (json_token_streq(jobj->js, t, key))
        {
            /* Value found. The next token has the value.
             * Populate the token pointer and return success.
             */
            t++;
            *val_t = t;
            return WM_SUCCESS;
        }
        else
        {
            /* Skip the value token since this is not the
             * key that we were looking for
             */
            t++;
            t = skip_to_last(t);
        }
    }
    return -WM_E_JSON_NOT_FOUND;
}

/* Search boolean value based on given key */
int json_get_val_bool(jobj_t *jobj, char *key, bool *value)
{
    jsontok_t *t;
    int ret = json_get_value(jobj, key, &t);
    if (ret != WM_SUCCESS)
        return ret;
    return json_str_to_bool(jobj, t, value);
}
/* Search integer value based on given key */
int json_get_val_int(jobj_t *jobj, char *key, int *value)
{
    jsontok_t *t;
    int ret = json_get_value(jobj, key, &t);
    if (ret != WM_SUCCESS)
        return ret;
    return json_str_to_int(jobj, t, value);
}
/* Search int64 value based on given key */
int json_get_val_int64(jobj_t *jobj, char *key, int64_t *value)
{
    jsontok_t *t;
    int ret = json_get_value(jobj, key, &t);
    if (ret != WM_SUCCESS)
        return ret;
    return json_str_to_int64(jobj, t, value);
}
/* Search float value based on given key */
int json_get_val_float(jobj_t *jobj, char *key, float *value)
{
    jsontok_t *t;
    int ret = json_get_value(jobj, key, &t);
    if (ret != WM_SUCCESS)
        return ret;
    return json_str_to_float(jobj, t, value);
}
/* Search string value based on given key */
int json_get_val_str(jobj_t *jobj, char *key, char *value, int maxlen)
{
    jsontok_t *t;
    int ret = json_get_value(jobj, key, &t);
    if (ret != WM_SUCCESS)
        return ret;
    return json_str_to_str(jobj, t, value, maxlen);
}

int json_get_val_str_len(jobj_t *jobj, char *key, int *len)
{
    jsontok_t *t;
    int ret = json_get_value(jobj, key, &t);
    if (ret != WM_SUCCESS)
        return ret;
    if (!t || t->type != JSMN_STRING)
        return -WM_E_JSON_INVALID_TYPE;
    *len = t->end - t->start;
    return WM_SUCCESS;
}
/* Search composite object based on given key */
int json_get_composite_object(jobj_t *jobj, char *key)
{
    jsontok_t *t;
    int ret = json_get_value(jobj, key, &t);
    if (ret != WM_SUCCESS)
        return ret;
    if (!t || t->type != JSMN_OBJECT)
        return -WM_E_JSON_INVALID_TYPE;
    /* Reduce the scope of subsequent searches to this object */
    jobj->cur = t;
    return WM_SUCCESS;
}

/* Release a composite object*/
int json_release_composite_object(jobj_t *jobj)
{
    if (jobj->cur->parent < 0)
        return -WM_E_JSON_FAIL;
    /* The parent of the current element will be its "key" */
    jobj->cur = &jobj->tokens[jobj->cur->parent];

    if (jobj->cur->parent < 0)
        return -WM_E_JSON_FAIL;
    /* The parent of the key will be the actual parent object/array */
    jobj->cur = &jobj->tokens[jobj->cur->parent];
    return WM_SUCCESS;
}

/* Search array object based on given key */
int json_get_array_object(jobj_t *jobj, char *key, int *num_elements)
{
    jsontok_t *t;
    int ret = json_get_value(jobj, key, &t);
    if (ret != WM_SUCCESS)
        return ret;
    if (!t || t->type != JSMN_ARRAY)
        return -WM_E_JSON_INVALID_TYPE;
    /* Reduce the scope of subsequent searches to this array */
    jobj->cur = t;
    /* Indicate the number of array elements found, if requested */
    if (num_elements)
        *num_elements = t->size;
    return WM_SUCCESS;
}
/* Release array object */
int json_release_array_object(jobj_t *jobj)
{
    return json_release_composite_object(jobj);
}

int json_array_get_num_elements(jobj_t *jobj)
{
    if (jobj->cur->type != JSMN_ARRAY)
        return -WM_FAIL;
    return jobj->cur->size;
}
/* Fetch the JSON value from an array based on index.
 * val_t is appropriately populated if the element is found
 * and WM_SUCCESS is returned. Else error is returned.
 */
static int json_get_array_index(jobj_t *jobj, uint16_t index, jsontok_t **val_t)
{
    *val_t = NULL;
    if (jobj->cur->type != JSMN_ARRAY)
        return -WM_E_JSON_INVALID_JARRAY;
    /* Given index exceeds the size of array. */
    if (index >= jobj->cur->size)
        return -WM_E_JSON_INVALID_INDEX;
    jsontok_t *t = jobj->cur;
    /* Incrementing once so that the token pointer points to index 0*/
    t++;
    while (index--)
    {
        /* For safety, check if the current token's end does not go
         * beyond the parent object's end. This case is unlikely, yet,
         * better to have a check.
         */
        if (t->end > jobj->cur->end)
            return -WM_E_JSON_FAIL;
        /* If the element is an array or object, skip to its last
         * element.
         */
        if (t->type == JSMN_ARRAY || t->type == JSMN_OBJECT)
            t = skip_to_last(t);
        t++;
    }
    *val_t = t;
    return WM_SUCCESS;
}

/* Search boolean value inside an array based on given index */
int json_array_get_bool(jobj_t *jobj, uint16_t index, bool *value)
{
    jsontok_t *t;
    int ret = json_get_array_index(jobj, index, &t);
    if (ret != WM_SUCCESS)
        return ret;
    return json_str_to_bool(jobj, t, value);
}
/* Search integer value inside an array based on given index */
int json_array_get_int(jobj_t *jobj, uint16_t index, int *value)
{
    jsontok_t *t;
    int ret = json_get_array_index(jobj, index, &t);
    if (ret != WM_SUCCESS)
        return ret;
    return json_str_to_int(jobj, t, value);
}
/* Search int64 value inside an array based on given index */
int json_array_get_int64(jobj_t *jobj, uint16_t index, int64_t *value)
{
    jsontok_t *t;
    int ret = json_get_array_index(jobj, index, &t);
    if (ret != WM_SUCCESS)
        return ret;
    return json_str_to_int64(jobj, t, value);
}
/* Search float value inside an array based on given index */
int json_array_get_float(jobj_t *jobj, uint16_t index, float *value)
{
    jsontok_t *t;
    int ret = json_get_array_index(jobj, index, &t);
    if (ret != WM_SUCCESS)
        return ret;
    return json_str_to_float(jobj, t, value);
}
/* Search string value inside an array based on given index */
int json_array_get_str(jobj_t *jobj, uint16_t index, char *value, int maxlen)
{
    jsontok_t *t;
    int ret = json_get_array_index(jobj, index, &t);
    if (ret != WM_SUCCESS)
        return ret;
    return json_str_to_str(jobj, t, value, maxlen);
}

int json_array_get_str_len(jobj_t *jobj, uint16_t index, int *len)
{
    jsontok_t *t;
    int ret = json_get_array_index(jobj, index, &t);
    if (ret != WM_SUCCESS)
        return ret;
    if (!t || t->type != JSMN_STRING)
        return -WM_E_JSON_INVALID_TYPE;
    *len = t->end - t->start;
    return WM_SUCCESS;
}

/* Search composite object inside an array based on given index */
int json_array_get_composite_object(jobj_t *jobj, uint16_t index)
{
    jsontok_t *t;
    int ret = json_get_array_index(jobj, index, &t);
    if (ret != WM_SUCCESS)
        return ret;
    if (!t || t->type != JSMN_OBJECT)
        return -WM_E_JSON_INVALID_TYPE;
    jobj->cur = t;
    return WM_SUCCESS;
}
/* Release the composite object inside the array */
int json_array_release_composite_object(jobj_t *jobj)
{
    if (jobj->cur->parent < 0)
        return -WM_E_JSON_FAIL;
    /* The parent of the current element will be the array */
    jobj->cur = &jobj->tokens[jobj->cur->parent];

    return WM_SUCCESS;
}

/* Search an array inside an array based on given index */
int json_array_get_array_object(jobj_t *jobj, uint16_t index, int *num_elements)
{
    jsontok_t *t;
    int ret = json_get_array_index(jobj, index, &t);
    if (ret != WM_SUCCESS)
        return ret;
    if (!t || t->type != JSMN_ARRAY)
        return -WM_E_JSON_INVALID_TYPE;
    jobj->cur     = t;
    *num_elements = t->size;
    return WM_SUCCESS;
}
/* Release the array */
int json_array_release_array_object(jobj_t *jobj)
{
    return json_array_release_composite_object(jobj);
}

/* Initialize the JSON parser */
static void json_obj_init(jobj_t *jobj, jsontok_t *tokens, int num_tokens)
{
    jobj->js         = NULL;
    jobj->tokens     = tokens;
    jobj->num_tokens = num_tokens;
    jobj->cur        = NULL;
    jsmn_init(&jobj->parser);
}

bool json_is_object(jobj_t *jobj)
{
    if (jobj->cur->type == JSMN_OBJECT)
        return true;
    else
        return false;
}
bool json_is_array(jobj_t *jobj)
{
    if (jobj->cur->type == JSMN_ARRAY)
        return true;
    else
        return false;
}

static bool __json_is_valid(jobj_t *jobj, jsmnenumtype_t type)
{
    jsontok_t *t = jobj->cur;
    /* If the current token type itself is not correct, return false*/
    if (t->type != type)
        return false;

    int num_children = t->size;
    /* If there are no children it is an error */
    if (num_children == 0)
    {
        return true;
    }

    while (num_children--)
    {
        /* Increment the token pointer first so that we begin from the
         * first token inside the object.
         */
        t++;
        /* For safety, check if the current token's end does not go
         * beyond the parent object's end. This case is unlikely, yet,
         * better to have a check.
         */
        if (t->end > jobj->cur->end)
            return false;
        if (jobj->cur->type == JSMN_OBJECT)
        {
            /* First token inside an object should be a key.
             * If not, it is an error.
             */
            if (t->type != JSMN_STRING)
                return false;
            /* If the key does not have a corresponding value,
             * or has multiple children, return an error.
             */
            if (t->size != 1)
                return false;
            /* Skip to value token so that we can validate the value
             */
            t++;
        }
        if ((t->type == JSMN_OBJECT) || (t->type == JSMN_ARRAY))
        {
            jsontok_t *tmp_tok = jobj->cur;
            jobj->cur          = t;
            bool valid         = __json_is_valid(jobj, t->type);
            jobj->cur          = tmp_tok;
            if (!valid)
                return false;
        }
        else if (t->size)
        {
            /* If the element is neither an object,
             * nor an array, it cannot have any children.
             * If it has, return error
             */
            return false;
        }
        t = skip_to_last(t);
    }
    return true;
}

static bool json_is_array_valid(jobj_t *jobj)
{
    return __json_is_valid(jobj, JSMN_ARRAY);
}

static bool json_is_object_valid(jobj_t *jobj)
{
    return __json_is_valid(jobj, JSMN_OBJECT);
}
/* Parse the given JSON string */
int json_init(jobj_t *jobj, jsontok_t *tokens, int num_tokens, char *js, int js_len)
{
    json_obj_init(jobj, tokens, num_tokens);
    int parsed_tokens = jsmn_parse(&jobj->parser, js, js_len, jobj->tokens, jobj->num_tokens);
    if (parsed_tokens < 0)
    {
        switch (parsed_tokens)
        {
            case JSMN_ERROR_NOMEM:
                return -WM_E_JSON_NOMEM;
            case JSMN_ERROR_INVAL:
                return -WM_E_JSON_INVAL;
            case JSMN_ERROR_PART:
                return -WM_E_JSON_INCOMPLETE;
            default:
                return -WM_E_JSON_FAIL;
        }
    }
    jobj->js         = js;
    jobj->num_tokens = parsed_tokens;
    jobj->cur        = jobj->tokens;
    /* TODO: Add some validation for JSON Object.
     * With earlier JSMN, for a valid JSON Object, number of
     * children would always be even numbered. But, this has changed
     * in new JSMN.
     */
    if (jobj->tokens->type == JSMN_OBJECT)
    {
        if (json_is_object_valid(jobj))
            return WM_SUCCESS;
        else
            return -WM_E_JSON_INVALID_JOBJ;
    }
    else if (jobj->tokens->type == JSMN_ARRAY)
    {
        if (json_is_array_valid(jobj))
            return WM_SUCCESS;
        else
            return -WM_E_JSON_INVALID_JARRAY;
    }
    else
        return -WM_E_JSON_INVALID_JOBJ;
}

int json_parse_start(jobj_t *jobj, char *js, int js_len)
{
    /* Passing NULL for tokens gives us the total number of tokens that
     * will be required to parse the string successfully.
     */
    json_obj_init(jobj, NULL, 0);
    int parsed_tokens = jsmn_parse(&jobj->parser, js, js_len, jobj->tokens, jobj->num_tokens);
    if (parsed_tokens < 0)
    {
        switch (parsed_tokens)
        {
            case JSMN_ERROR_NOMEM:
                return -WM_E_JSON_NOMEM;
            case JSMN_ERROR_INVAL:
                return -WM_E_JSON_INVAL;
            case JSMN_ERROR_PART:
                return -WM_E_JSON_INCOMPLETE;
            default:
                return -WM_E_JSON_FAIL;
        }
    }
    jsontok_t *tokens = os_mem_calloc(parsed_tokens * sizeof(jsontok_t));
    if (!tokens)
        return -WM_E_JSON_NOMEM;

    int ret = json_init(jobj, tokens, parsed_tokens, js, js_len);
    if (ret != WM_SUCCESS)
        json_parse_stop(jobj);

    return ret;
}

void json_parse_stop(jobj_t *jobj)
{
    if (jobj->tokens)
        os_mem_free(jobj->tokens);
}
