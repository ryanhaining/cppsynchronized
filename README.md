CPP Synchronized
================

A fun little project of mine.  When I first discovered Java's `synchronized`
blocks, I was intrigued.  It's pretty easy to forget to unlock a mutex, and
easier to follow the structure of a block.  I thought for a while about how I
might implement something like this in C++11, and eventually accomplished this
in two different ways.

synctable
---------
My favorite of the two implementations.  Allows for a synchronized block on
any pointer using the syntax:

    Object obj;
    synchronized(&obj) {
        // ... critical section
    }

Note that this could have been done using an object rather than a point with
something more resembling Java `synchronized(obj)` but it seemed to be more in
the spirit of C++ to use the address.

This is accomplished with a `#define`, and a `for` loop that initializes one
object to lock and unlock a mutex on for entry/exit.  The mutexes themselves
are stored in a global `std::unordered_map` of `void *` -> `mutex *`.  
