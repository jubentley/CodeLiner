# CodeLiner

Very old, made it during the holidays at uni, is and designed for only C in C++, made in VS15 if not earlier, comes with no guarantees 😁

Uses Win32API (90's C windows API) to scour a directory for .cpp and .h files and a nifty algorithm to detect valid lines-of-code with a broad emphasis on counting multi-statement lines in those files (found at /Codeliner/CodeInspector.cpp).

At some point in time I wanted to make it highly threaded (one thread per file, per CodeInspector) and instead of mutex ++'ing the threads I thought I would make an array to hold the LOC values of each file, and I decided to "fight the compiler" to make that array dynamic (non compile time length), cause how am I supposed to know how many files there are, right?

I installed the infrastructure for threading but never implemented.
