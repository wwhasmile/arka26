#ifndef HAS_IO_STREAM_H
#define HAS_IO_STREAM_H

#include <defines.h>

typedef enum {
	STREAM_MODE_R,
	STREAM_MODE_W,
	STREAM_MODE_RW,
	STREAM_MODE_RB,
	STREAM_MODE_WB,
	STREAM_MODE_RWB,
	STREAM_MODE_ENUM_COUNT
} StreamMode;

typedef enum {
	STREAM_WHENCE_SET,
	STREAM_WHENCE_CUR,
	STREAM_WHENCE_END,
	STREAM_WHENCE_ENUM_COUNT
} StreamWhence;

typedef struct Stream Stream;

typedef struct {
	i32 (*read)(Stream *file, void *buffer, u32 count);
	i32 (*write)(Stream *file, const void *buffer, u32 count);
	i32 (*seek)(Stream *file, i32 to, StreamWhence whence);
	i32 (*tell)(Stream *file);
	i32 (*size)(Stream *file);
	bool (*close)(Stream *file);
} StreamInterface;

static inline i32 File_Read(Stream *file, void *buffer, u32 count);
static inline i32 File_Write(Stream *file, const void *buffer, u32 count);
static inline i32 File_Seek(Stream *file, i32 to, StreamWhence whence);
static inline i32 File_Tell(Stream *file);
static inline bool File_Close(Stream *file);

static inline i32 File_Read(Stream *file, void *buffer, u32 count)
{
	StreamInterface *interface = (StreamInterface*)file;
	if (interface->read == NULL)
		return -1;
	return interface->read(file, buffer, count);
}

static inline i32 File_Write(Stream *file, const void *buffer, u32 count)
{
	StreamInterface *interface = (StreamInterface*)file;
	if (interface->write == NULL)
		return -1;
	return interface->write(file, buffer, count);
}

static inline i32 File_Seek(Stream *file, i32 to, StreamWhence whence)
{
	StreamInterface *interface = (StreamInterface*)file;
	if (interface->seek == NULL)
		return -1;
	return interface->seek(file, to, whence);
}

static inline i32 File_Tell(Stream *file)
{
	StreamInterface *interface = (StreamInterface*)file;
	if (interface->tell == NULL)
		return -1;
	return interface->tell(file);
}

static inline bool File_Close(Stream *file)
{
	StreamInterface *interface = (StreamInterface*)file;
	if (interface->close == NULL)
		return -1;
	return interface->close(file);
}

#endif // HAS_IO_STREAM_H
