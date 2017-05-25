# Password generator/manager

This is some very simple code to take a master password/passphrase,  run
it through SHA-256,  and generate passwords.  All this is done on the
host machine,  eliminating the need to trust third parties with passwords
(or to store passwords at all).  It is based heavily on

http://www.movable-type.co.uk/scripts/sha256.html

and a *very* big tip of the hat should go to Chris Veness for providing
that code.  (I'm really a C guy,  not a Javascript guy.)  Chris' code
crunched an input message and displayed the SHA-256 hash for it;  I
extended this a bit so that the hash could be expressed as a password.

You can give it a test run at

https://www.projectpluto.com/pw_hash/pw_hash.htm

However,  I'd recommend that you just test it out there (don't enter any
sensitive passwords.)  For 'real' use,  you should save `pw_hash.htm` and
`sha256.js` on your own machine,  and run it from there.  This provides
some forward protection in case the version at the above URL is compromised.

# How it works

If you look at `pw_hash.htm`,  you'll see that I extended Chris' original
form,  which just showed the SHA-256 hash in hexadecimal.  I kept that,
largely to make sure nothing I did broke that code (you can type in a
message of 'abc' and verify that the hash is correct.)

But to this,  I've added a series of output boxes for passwords.  For each
password,  a maximum length and a list of possible "special characters" is
given.  I do make the assumption that any system can handle upper and
lowercase letters and digits.  Adding/revising these shouldn't be too
difficult.  You'll also see that for each site,  a different "salt" is added
before and after your password;  you should change these.

The input message can be *anything*,  including spaces,  carriage returns,
and Unicode outside the realm of ASCII,  or even "extended ASCII".  (You will
note that the salts include Greek letters and mathematical symbols,  and
I'd recommend doing that.)

# Known limitations

(1) If a site requires a numeric password,  see the '15-digit PIN' example:
in it,  we ask for a fifteen-digit password consisting only of numbers.
Similarly,  GitHub requires at least one number,  so we generate one
"special" character,  one digit,  and then the rest of the password (which
may or may not contain more specials or digits,  but we've at least
guaranteed we met the required minimum.)

(2) The passwords are very slightly non-random.  For example,  suppose you
have no special characters,  so there are 62 possible characters in a
password. We take a 32-bit pseudorandom number from the SHA-256 result,
modulo 62. Since 2^32 isn't evenly divisible by 62,  smaller outputs are
(*very* slightly) more likely than larger ones.  The difference is small
enough that I don't expect to do anything about it.

(3) Everything is displayed on-screen,  with no efforts to avoid shoulder
surfing.  Output probably lingers in RAM.  If I knew how to do it,  I'd
try to make sure there was a way to overwrite the message and passwords
with zeroes or garbage.  Note that this is somewhat of a theoretical point;
if somebody has access to your device's RAM,  they can probably intercept
it when the password goes out to the site in question.

(4) A button to copy the text to the clipboard would be nice.

 (5) If you have to change your password for a site,  I'd recommend just
editing `pw_hash.htm` and modifying the salt used for that site.  (Which
you should do before using this anyway;  for all you know,  I work for the
NSA or GCHQ or their Russian or Chinese counterparts and picked those salts
*very* carefully.)
