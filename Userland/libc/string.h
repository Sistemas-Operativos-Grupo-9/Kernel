#pragma once

#include "stdint.h"
#include <stddef.h>

uint64_t strlen(const char *str);
void strcpy(char *dst, const char *src);
void strncpy(char *dst, const char *src, size_t n);
int strcmp(const char *str1, const char *str2);
int strncmp(const char *s1, const char *s2, size_t n);
size_t tokencount(const char *str, char c);
size_t tokencountn(const char *buf, char c, size_t n);
/**
 * @brief Does not allocate memory.
 * Destroys str.
 * @return a pointer to the trimmed string
 */
char *trim(char *str);
/**
 * @brief splits str into an array of strings according to the given delimiter
 * delim.
 *
 *  If does not contain delim, returns an array of 1 element with str
 * copied into it. If the first or last character of str is delim, it ignores
 * it. If there are two or more delims together, it treats them as one.
 *
 * This function allocates memory for the return value, so this must be
 * freed later by the user.
 *
 * @param str the input string
 * @param delim the delimiter to split str
 * @param size the size of the resulting array
 * @return an array of strings with the splitted input string, NULL on error
 */
char **split(const char *str, char delim, size_t *size);

/**
 * @brief splits str contents like the split function but without allocating
 * memory. It saves the pointers to the split strings in splitArray and destroys
 * str in the proces, so it cannot be used after a split2 call
 *
 * @return the size of the resulting array.
 */
size_t split2(char *str, char delim, char **splitArray);

/**
 * @brief filters from str all appearences of any token in tokens and stores the
 * result on dest
 *
 * @param str input string
 * @param tokens list of tokens to filter (each as char *) terminated in NULL
 * @param dest pointer to store the string based on str but without any token
 * from tokens
 * @returns size of filtered string (without counting the final '\0')
 */
size_t filterTokens(char *buf, char **tokens, char *dest);

/**
 * @brief filters from buf all appearences of any charachter in tokens and
 * stores the result on dest
 *
 * @param buf input buffer
 * @param tokens list of tokens to filter (each char of the string)
 * @param n max size
 * @param dest pointer to store the string based on str but without any token
 * from tokens
 * @returns size of filtered buffer
 */
size_t filterCharsN(char *buf, char *tokens, size_t n, char *dest);

void deleteCharAtIndex(char *str, size_t index);
// returns the amount of deleted characters
size_t deleteNCharsAtIndex(char *str, size_t index, size_t n);

void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);
void *memmove(void *s1, const void *s2, uint64_t n);
int memcmp(const void *ptr1, const void *ptr2, uint64_t num);