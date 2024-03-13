# Libax – c++ cross-platform library

Back in 2008 cross-platform was a pain prior C++11, for example thread, atomic integer, data time, are all different on platforms.
And also helpers classes/functions were absent, such as string formatter, scoped_lock.

## Features overview:
- File System – native file api wrapper, support memory mapping file, Unicode filename
- Multi-thread - C++ classes for Thread, Mutex, CondVar, Atomic integer
- Data Containers
	- axArray<T,N> - Array with inline buffer – similar to eastl::fixed_vector<T,N>, llvm SmallVector<T,N> one unique feature is single interface for any size of inline buffer by class IArray<T>
	- axDList<T> – intrusive doubly linked list, similar to eastl::intrusive_list
- File Format Parser – Json parser, CSV parser, all supported UTF-8
- Linear algebra – Vector2/3/4, matrix, ray cast
- DataTime / System Time – parse from string, high resolution timer
- Network – C++ class for BSD socket, and unified difference between windows / unix socket
- Database - connect various database with single interface (support MySQL, SQLite, MS SQL, Oracle
- String – UTF-8 / wchar_t conversion, type safety formatter (similar to libfmt)
- Error handling – Logging system, error code management 