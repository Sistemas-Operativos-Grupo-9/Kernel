#pragma once

#include "stdint.h"
#include <stddef.h>

uint64_t strlen(char *str);
void strcpy(char *dst, const char *src);
void strncpy(char *dst, const char *src, size_t n);
int strcmp(char *str1, char *str2);
int strncmp(const char *s1, const char *s2, size_t n);
size_t tokencount(char *str, char c);
/**
 * @brief splits str into an array of strings according to the given delimiter
 * delim.
 * If str does not contain delim, returns an array of 1 element with str copied
 * into it. If the first or last character of str is delim, it ignores it. If
 * there are two or more delims together, it treats them as one.
 *
 * @param str the input string
 * @param delim the delimiter to split str
 * @param size the size of the resulting array
 * @return an array of strings with the splitted input string
 */
char **split(char *str, char delim, size_t *size);
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
size_t filterTokens(char *str, char **tokens, char *dest);

/**
 * @brief filters from str all appearences of any charachter in tokens and
 * stores the result on dest
 *
 * @param str input string
 * @param tokens list of tokens to filter (each char of the string)
 * @param dest pointer to store the string based on str but without any token
 * from tokens
 * @returns size of filtered string (without counting the final '\0')
 */
size_t filterChars(char *str, char *tokens, char *dest);

void deleteCharAtIndex(char *str, size_t index);
// returns the amount of deleted characters
size_t deleteNCharsAtIndex(char *str, size_t index, size_t n);

void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);
void *memmove(void *s1, const void *s2, uint64_t n);
int memcmp(const void *ptr1, const void *ptr2, uint64_t num);