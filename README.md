habanero
========
Open repo with some useful stuff I produce sometimes.


cpp/tiny_string
-----------
tiny_string is a std::string implementation with a small sizeof - 8 bytes. It assumes the following:
  * platform with 64-bit pointers (for my case it is x86-64)
  * little endian
  * only char as a value type, not a template class
  * no allocators
  * nearly C++14 compatible

The reason why this class exists is a situation when you need to store a string in an object and most of a times it will be empty. With this case basically there are two options:
  * use straightforward std::string, which is 24 bytes long for example on clang's std::string implemetation. Wasting a lot of memory for nothing.
  * use C-style char* buffer - 8 bytes for pointer. Manually allocate and deallocate it, deal with copying/moving etc. Boring and error-prone.

So tiny_string can be used - for strings up to 6 characters it will use a built-in buffer and will use malloc/realloc for larger strings.

