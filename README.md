# Password generator/manager

This is some very simple code to take a master password/passphrase,  run
it through SHA-256,  and generate passwords.  It is based heavily on

http://www.movable-type.co.uk/scripts/sha256.html

and a *very* big tip of the hat should go to Chris Veness for providing
that code.  (I'm really a C guy,  not a Javascript guy.)  Chris' code
crunched an input message and displayed the SHA-256 hash for it;  I
extended this a bit so that the hash could be expressed as a password.

**Don't just enter a master password into the form on this page**.  It should
be,  at least,  difficult for that password to be snatched in transit,  or
for someone to hack GitHub so they can look at incoming master passwords.
But it would be entirely possible.  So save `pw_hash.htm` and `sha256.js`
on your own machine,  and run it from there.  (I quite deliberately avoided
making use of third-party scripts.)

# How it works

If you look at `pw_hash.htm`,  you'll see that I extended Chris' original
form,  which just showed the SHA-256 hash in hexadecimal.  I kept that,
largely to make sure nothing I did broke that code (you can type in a
message of 'abc' and verify that the hash is correct.)

But to this,  I've added a series of output boxes for passwords.  For each
password,  a maximum length and a list of possible "special characters" is
given.  I do make the assumption that any system can handle upper and lowercase
letters and digits.  Adding/revising these shouldn't be too difficult.  You'll
also see that for each site,  a different "salt" is added before and after
your password;  you should change these.

The input message can be *anything*,  including spaces,  carriage returns,
and Unicode outside the realm of ASCII,  or even "extended ASCII".  (You will
note that the salts include Greek letters and mathematical symbols,  and
I'd recommend doing that.)

# Known limitations

(1) If a site can't handle passwords with upper and lowercase letters and
digits,  this project won't work well for you.  (And the operators of the
site need to be yelled at.)

(2) If a site *requires* passwords have at least one digit,  or at least one special
character,  or upper and lowercase letters,  this won't help you.  (This
could be fixed in any of several ways:  for example,  generate your password
two characters shy of the maximum and then add '5&' to it before displaying it.
Better would be to have a function that generates a password and checks to
make sure it fulfills the rules;  if it doesn't,  generate new password(s)
until it gets it right.)

(3) The passwords are very slightly non-random.  For example,  suppose you have
no special characters,  so there are 62 possible characters in a password.
We take a 32-bit pseudorandom number from the SHA-256 result,  modulo 62.
Since 2^32 isn't evenly divisible by 62,  smaller outputs are (*very* slightly)
more likely than larger ones.  The difference is small enough that I don't
expect to do anything about it.

(4) Everything is displayed on-screen,  with no efforts to avoid shoulder
surfing.  Output probably lingers in RAM;  if I knew how to do it,  I'd
try to make sure there was a way to overwrite the message and passwords
with zeroes or garbage.

(5) A button to copy the text would be nice.
