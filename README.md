# Password generator/manager

This is some very simple code to take a master password/passphrase,
run it through SHA-256,  and generate passwords for assorted accounts.
Thus,  you need know only one password,  and use that to generate
passwords for everything else.

All this is done on the host machine,  eliminating the need to trust
third parties (including me) with passwords.  Your password is never
actually stored,  even on your own machine;  if somebody accesses
your device,  they still don't have your passwords.

You can give it a test run at

https://www.projectpluto.com/pw_hash/pw_hash.htm

However,  I'd recommend that you just test it out there (don't enter any
sensitive passwords.)  For 'real' use,  you should save `pw_hash.htm` and
`sha256.js` on your own machine,  and run it from there.  This provides
some forward protection in case the version at the above URL is compromised.
It also allows you to change the salts currently provided in `pw_hash.htm`,
providing some further level of security.

(I think in theory,  you ought to be fine even without making a private
copy.  The greatest risk would be that the NSA/FSB/GCHQ/Illuminati might
"modify" the version I posted in some manner that enables them to see what
you're typing.)

The code is heavily based on

http://www.movable-type.co.uk/scripts/sha256.html

and a *very* big tip of the hat should go to Chris Veness for providing
that code.  (I'm really a C guy,  not a Javascript guy.)  Chris' code
crunched an input message and displayed the SHA-256 hash for it;  I
extended this a bit so that the hash could be expressed as a password.

# Why another password manager?

I wanted something that eliminates the need for you to trust the author.
With this scheme, nothing you type leaves your machine.  If the NSA
tries rubber-hose cryptanalysis,  I still can't tell them your passwords.

# How it works

If you look at `pw_hash.htm`,  you'll see that I extended Chris' original
form,  which just showed the SHA-256 hash in hexadecimal.  I kept the
SHA-256 display, largely to make sure nothing I did broke that code (you
can type in a message of 'abc' and verify that the hash is correct.)

But to this,  I've added a series of output boxes for passwords.  For each
password,  a maximum length and a list of possible "special characters" is
given.  You'll also see that for each site,  a different "salt" is added
before and after your password;  you should change these,  either by hand
(should be enough to change a few characters per salt) or by running `resalt`
(for which a C source and makefile are provided).

The input message can be *anything*,  including spaces,  carriage returns,
and Chinese,  Russian,  Farsi,  mathematical symbols,  etc.  (Which works
fine from a laptop or desktop.  Entering,  say,  ⅔ from a cellphone may
be a problem.)

# Known limitations

(1) If a site requires a numeric password,  see the '15-digit PIN' example:
in it,  we ask for a fifteen-"character" password consisting only of numbers.
Similarly,  GitHub requires at least one number,  so we generate one
"special" character,  one digit,  and then the rest of the password (which
may or may not contain more specials or digits,  but we've at least
guaranteed we met the required minimum.)

At some point,  I may provide functions which will compute a password,
then rehash it if it flunks site-specific rules until eventually you get
one that passes.  (Which usually won't be all that long;  set a maximum
length password,  for example,  and it will almost always contain at least
one number and one "special" character,  just by chance.)

(2) Everything is displayed on-screen,  with no efforts to avoid shoulder
surfing.  Output probably lingers in RAM.  If I knew how to do it,  I'd
try to make sure there was a way to overwrite the message and passwords
with zeroes or garbage.  Note that this is somewhat of a theoretical point;
if somebody has access to your device's RAM,  you're presumably thoroughly
compromised anyway.  Wiping RAM does offer some advantages in some unusual
cases,  though.

(3) If you have to change your password for a site,  I'd recommend just
editing `pw_hash.htm` and modifying the salt used for that particular site.
You will then get a new password for just that site,  leaving the others
unharmed.

(4) A button to copy the text to the clipboard would be nice.  At present,
you have to type in your master password,  click on the generated password
you want,  and copy and paste it.

(5) Some sites with truly bizarre/stupid requirements may cause trouble.
Samsung(R),  for example,  disallows having three identical characters
in a row (no 'Password111') or three consecutive characters (no
'Password123').  (It also has a maximum password length of 15 characters,
indicating they store your passwords in cleartext!  But I digress.)

If you fall afoul of such requirements,  the only fix (at present) is to
modify the salt for that password.

(6) The passwords are very slightly non-random.  For example,  suppose you
have no special characters,  so there are 62 possible characters in a
password. We take a 32-bit pseudorandom number from the SHA-256 result,
modulo 62. Since 2^32 isn't evenly divisible by 62,  smaller outputs are
(*very* slightly) more likely than larger ones.  The difference is small
enough that I don't expect to do anything about it.  (In a very long password,
some characters near the end may be biased by less than 1%.)

