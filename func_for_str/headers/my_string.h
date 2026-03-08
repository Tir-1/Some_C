#ifndef MY_STRING_H
#define MY_STRING_H
#include <stddef.h>
size_t my_strlen(const char *s);
char *get_str();
char *my_strdup(const char* str);
char *my_strtok(char *str, const char *sep, size_t *index, size_t *word_len);
size_t my_poisk(const char *str, int c);
void my_memcpy(char* str, const char *source, size_t len);
#endif
