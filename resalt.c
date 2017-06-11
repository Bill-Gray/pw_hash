#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <locale.h>

/* A little utility to replace the 'salt' strings of pw_hash.htm with
random new salts.  In theory,  if this project really caught on,  it
might become worth someone's while to create rainbow tables using the
default salts.  For the truly paranoid people with tinfoil chapeaux,
you'd want to at least modify those salts (changing a few characters
in each salt would do the trick) or just run this program.

   It reads through pw_hash.htm,  finds the salt strings,  and
replaces them using bytes read from /dev/urandom (computationally
secure pseudo-random number generator,  available in Linux,  *BSD,
and I think OS/X),  writing the result to stdout.

   Command line options are

   -s(n1,n2)      'Leading' salt will have length n1,  'trailing' n2.
                  Defaults are 7 and 5,  respectively.
   -a             Use ASCII characters only.  (Default will include
                  some "unusual" extended Latin characters in the salts.)
*/


/* Generate a random string of the desired length.  The random
bytes read in will be remapped to run from 0x20 = 32 = ' ' to
0x7e = 126 = '~'.  Anything past that gets remapped from 0xb0 = 176
to 0x180 = 336 ("extended Latin" characters).  A few bytes of such
text from the Linux PRNG ought to be enough to salt anything. */

FILE *ofile;

static void show_salt( FILE *ifile, int count, bool use_non_ascii)
{
   while( count)
      {
      int c = fgetc( ifile);

      assert( c != EOF);
      if( c < 0)
         c += 256;
      c += 0x20;     /* unicode starts at 0x20 = space */
      if( c > 0x7e)  /* and has a gap between 0x7e='~' and 0xb0 */
         c += 0xb0 - 0x7e;
      if( c != '"' && c != '\\' && (c <= '~' || use_non_ascii))
         {
         fprintf( ofile, "%lc", (wchar_t)c);
         count--;
         }
      }
}

int main( const int argc, const char **argv)
{
   char buff[200], *tptr;
   FILE *ifile = fopen( "pw_hash.htm", "rb");
   FILE *random_file = fopen( "/dev/urandom", "rb");
   int leading_salt = 7, trailing_salt = 5, i;
   bool use_non_ascii = true;

   setlocale(LC_ALL, "");
   assert( ifile);
   ofile = stdout;
   for( i = 1; i < argc; i++)
      if( argv[i][0] == '-')
         switch( argv[i][1])
            {
            case 's':
               sscanf( argv[i] + 2, "%d,%d", &leading_salt, &trailing_salt);
               break;
            case 'a':
               use_non_ascii = false;
               break;
            case 'o':
               {
               const char *ofilename = argv[i] + 2;

               if( !*ofilename && i < argc - 1)
                  ofilename = argv[i + 1];
               ofile = fopen( ofilename, "wb");
               if( !ofile)
                  {
                  fprintf( stderr, "Couldn't open output file %s: ", ofilename);
                  perror( "");
                  return( -2);
                  }
               }
               break;
            default:
               printf( "Unrecognized command line option '%s'\n", argv[i]);
               return( -1);
            }
   while( fgets( buff, sizeof( buff), ifile))
      {
      tptr = strstr( buff, "Sha256.hash( \"");
      if( !tptr)
         fprintf( ofile, "%s", buff);
      else
         {
         tptr[14] = '\0';
         fprintf( ofile, "%s", buff);
         show_salt( random_file, leading_salt, use_non_ascii);
         tptr += 15;
         while( *tptr && *tptr != '"')
            tptr++;
         assert( *tptr == '"');
         assert( !memcmp( tptr, "\" + this.value + \"", 18));
         tptr[18] = '\0';
         fprintf( ofile, "%s", tptr);
         tptr += 19;
         show_salt( random_file, trailing_salt, use_non_ascii);
         while( *tptr && *tptr != '"')
            tptr++;
         assert( *tptr == '"');
         fprintf( ofile, "%s", tptr);
         }
      }
   fclose( ifile);
   return( 0);
}
