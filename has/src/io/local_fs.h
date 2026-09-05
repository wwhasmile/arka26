#ifndef HAS_IO_LOCAL_FS_H
#define HAS_IO_LOCAL_FS_H

#include <io/fs.h>

Fs *LocalFs_Init(const char *root);

bool LocalFs_Exists(Fs *fs, const char *file);

Stream *LocalFs_Open(Fs *fs, const char *file, StreamMode mode);

i32 LocalFs_FileSize(Fs *fs, const char *file);

i32 LocalFs_Release(Fs *fs);

#endif // HAS_IO_LOCAL_FS_H
