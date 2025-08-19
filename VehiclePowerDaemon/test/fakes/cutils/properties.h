/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef CUTILS_PROPERTIES_H
#define CUTILS_PROPERTIES_H

#define PROPERTY_VALUE_MAX 92

#ifdef __cplusplus
extern "C" {
#endif

int property_get(const char* key, char* value, const char* default_value);
int property_set(const char* key, const char* value);

#ifdef __cplusplus
}
#endif

#endif  // CUTILS_PROPERTIES_H
