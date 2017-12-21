![SQLite Edit Logo](/img/sqlite-edit.png)

# SQLite Edit: A lightweight SQLite editor.

## Compile (Windows VS2017)

### Download Libraries
* [SQLite 3.21.0 Amalgamation](https://www.sqlite.org/2017/sqlite-amalgamation-3210000.zip)
* [SQLite 3.21.0 Windows 32-bit DLL](https://www.sqlite.org/2017/sqlite-dll-win32-x86-3210000.zip)
* [SQLite 3.21.0 Windows 64-bit DLL](https://www.sqlite.org/2017/sqlite-dll-win64-x64-3210000.zip)

### Install Libraries
1. Extract sqlite-amalgamation-3210000.zip\sqlite3.h to lib\sqlite3\include
2. Extract sqlite-dll-win32-x86-3210000.zip to lib\sqlite3\x86
3. From "Developer Command Prompt for VS 2017" run LIB /DEF:sqlite3.def in lib\sqlite3\x86
4. Extract sqlite-dll-win64-x64-3210000.zip to lib\sqlite3\x64
5. From "Developer Command Prompt for VS 2017" run LIB /DEF:sqlite3.def in lib\sqlite3\x64

### Build
Run build\vs2017\sqlite-edit.sln and build.
