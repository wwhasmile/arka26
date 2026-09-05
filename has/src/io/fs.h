#ifndef HAS_IO_FS_H
#define HAS_IO_FS_H

#include "stream.h"

typedef struct Fs Fs;

typedef struct {
	bool (*exists)(Fs *fs, const char *file);
	Stream *(*open)(Fs *fs, const char *file, StreamMode mode);
	i32 (*fileSize)(Fs *fs, const char *file);
	i32 (*release)(Fs *fs);
} FsInterface;

static inline bool Fs_Exists(Fs *fs, const char *file);
static inline Stream *Fs_Open(Fs *fs, const char *file, StreamMode mode);
static inline i32 Fs_FileSize(Fs *fs, const char *file);
static inline i32 Fs_Release(Fs *fs);

static inline bool Fs_Exists(Fs *fs, const char *file)
{
	FsInterface *interface = (FsInterface*)fs;
	return interface->exists(fs, file);
}

static inline Stream *Fs_Open(Fs *fs, const char *file, StreamMode mode)
{
	FsInterface *interface = (FsInterface*)fs;
	return interface->open(fs, file, mode);
}

static inline i32 Fs_FileSize(Fs *fs, const char *file)
{
	FsInterface *interface = (FsInterface*)fs;
	return interface->fileSize(fs, file);
}

static inline i32 Fs_Release(Fs *fs)
{
	FsInterface *interface = (FsInterface*)fs;
	return interface->release(fs);
}

#endif // HAS_IO_FS_H
