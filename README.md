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

```c++
synchronized(&obj) {
    // ... critical section
}
```

Note that this could have been done using an object rather than a pointer with
something more resembling Java `synchronized(obj)` but it seemed to be more in
the spirit of C++ to use the address.

This is accomplished with a `#define`, and a `for` loop that initializes an
object to lock and unlock a `std::mutex` on `for` entry/exit.  The mutexes
themselves are stored in a global `std::unordered_map` of
`void *` -> `mutex *`.

An alternative use is a `tablesynchronized` block.  This requires the
programmer to create a `SyncTable` and pass that as the first argument to
`tablesynchronized`

    SyncTable table;
    // ... later on
    tablesynchronized(table, &obj) {
        // ... critical section
    }

The idea here is a group of threads can use a local `SyncTable` rather than
the global one to avoid contention on the global table.


lockable_objects
----------------
A non-intrusive use of multiple inheritance.  A "lockable" version of any class
can be instantiated with

    Lockable<Object> lockable_object(/* Object ctor args */);

Any lockable object can be used within a `synchronized` block.  This approach
avoids the `SyncTable`s entirely

    synchronized(lockabe_object){
        // ... critical section
    }

Can then be used without the overhead of a lookup for the mutex.  Additionally,
no `&` is used for lockable objects.
