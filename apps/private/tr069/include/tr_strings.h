/*!
 * \file strings.h
 *
 * Copyright (C) Arthur Echo(arthur.echo@gmail.com)
 */
#ifndef __HEADER_TR_STRINGS__
#define __HEADER_TR_STRINGS__

char *skip_blanks( const char *str );
char *skip_non_blanks( const char *str );
char *trim_blanks( char *str );
int string_is_digits( const char *str );
int string_start_with( const char *str, char with, int skip_leading_space );
int string2boolean( const char *str );

#endif
