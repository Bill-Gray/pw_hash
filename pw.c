/* compile with gcc -o pw pw.c sha256.c
(can/should add -Wall -Wextra -pedantic -O3 options)

See 'ex_hash.txt' for some details.  Basically,  given a master
password on the command line,  this runs it and salts through
SHA256 to generate passwords.  You can,  in 'ex_hash.txt',  specify
the lengths and characters available to the passwords (i.e.,  some
sites disallow 'special' characters) and required characters (i.e.,
"password must have a digit and a special character").  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include "sha256.h"

static void hash_to_pwd( const unsigned char *hash, size_t pwd_len, const char *pw_chars,
            char *password)
{
   unsigned radix = (unsigned)strlen( pw_chars), x = 0;
   unsigned mod = 1;

   while( pwd_len)
      {
      mod *= 256;
      x = x * 256 + *hash++;
      while( pwd_len && x / radix < mod / radix)
         {
         *password++ = pw_chars[x % radix];
         pwd_len--;
         mod /= radix;
         x /= radix;
         }
      }
   *password = '\0';
}

static void search_and_replace( char *text, const char *old_text,
            const char *new_text)
{
   while( (text = strstr( text, old_text)) != NULL)
      {
      const size_t old_len = strlen( old_text);
      const size_t new_len = strlen( new_text);

      memmove( text + new_len, text + old_len, strlen( text) - old_len + 1);
      memcpy( text, new_text, new_len);
      }
}

static void make_substitutions( char *text, const char **subs)
{
   size_t i;

   for( i = 0; subs[i]; i += 2)
      search_and_replace( text, subs[i], subs[i + 1]);
}

/* If 'required' is,  say,

0123456789 !@#$%*(^)

   this function will check to make sure there's at least one digit
and one 'special' character.  */

static bool is_ok_password( const char *pwd, const char *required)
{
   while( *required)
      {
      bool match_found = false;

      while( *required > ' ')
         {
         if( !match_found)
            match_found = (strchr( pwd, *required) != NULL);
         required++;
         }
      if( !match_found)
         return( false);
      while( *required == ' ')
         required++;
      }
   return( true);
}

int main( const int argc, const char **argv)
{
   char salt[64], buff[200], must_have[200];
   char *subs[100];
   int n_subs = 0;
   FILE *ifile = fopen( "pw_hash.txt", "rb");
   size_t len = 0;

   if( !ifile)
      {
      ifile = fopen( "ex_hash.txt", "rb");
      printf( "Showing 'example' passwords\n");
      }
   assert( ifile);
   *must_have = '\0';
   subs[0] = NULL;
   while( fgets( buff, sizeof( buff), ifile))
      {
      char *text = buff + 12;

      *strchr( buff, 10) = '\0';
      if( !memcmp( buff, "Sub ", 4))
         {
         subs[n_subs * 2] = (char *)malloc( 10);
         subs[n_subs * 2 + 1] = (char *)malloc( strlen( buff + 12));
         strcpy( subs[n_subs * 2 + 1], buff + 13);
         sscanf( buff + 4, "%9s", subs[n_subs * 2]);
         n_subs++;
         subs[n_subs * 2] = NULL;
         }
      if( !memcmp( buff, "Salt ", 5))
         memcpy( salt, text, 64);
      if( !memcmp( buff, "Site ", 5))
         printf( "%-30s", text);
      if( !memcmp( buff, "Lengt", 5))
         len = (size_t)atoi( text);
      if( !memcmp( buff, "Must-", 5))
         {
         strcpy( must_have, text);
         make_substitutions( must_have, (const char **)subs);
         }
      if( !memcmp( buff, "Text ", 5))
         {
         char pwd[80];
         unsigned char hash[SHA256_SUM_LEN];
         int i;
         sha256_context sh;

         sha256_starts( &sh);
         sha256_update( &sh, (const uint8_t *)salt, sizeof( salt));
         for( i = 1; i < argc; i++)
            if( argv[i][0] != '-')
               sha256_update( &sh, (const uint8_t *)argv[i], strlen( argv[i]));
         sha256_finish( &sh, hash);

         make_substitutions( text, (const char **)subs);
         search_and_replace( text, " ", "");
         hash_to_pwd( hash, len, text, pwd);
         while( !is_ok_password( pwd, must_have))
            {
            sha256_starts( &sh);
            sha256_update( &sh, (const uint8_t *)hash, sizeof( hash));
            sha256_update( &sh, (const uint8_t *)pwd, strlen( pwd));
            sha256_finish( &sh, hash);
            hash_to_pwd( hash, len, text, pwd);
            }
         printf( "%s\n", pwd);
         *must_have = '\0';
         }
      }
   fclose( ifile);
   return 0;

}
