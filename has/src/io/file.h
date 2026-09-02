#ifndef HAS_IO_FILE_H
#define HAS_IO_FILE_H

#include "stream.h"

Stream *File_Open(const char *filename, StreamMode mode);

i32 File_Read(Stream *stream, void *buffer, u32 count);
i32 File_Write(Stream *stream, const void *buffer, u32 count);

i32 File_Seek(Stream *stream, i32 to, StreamWhence whence);
i32 File_Tell(Stream *stream);
i32 File_Size(Stream *stream);

i32 File_Flush(Stream *stream);
i32 File_Close(Stream *stream);

#endif // HAS_IO_FILE_H
