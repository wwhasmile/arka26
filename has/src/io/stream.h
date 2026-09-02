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
	i32 (*read)(Stream *stream, void *buffer, u32 count);
	i32 (*write)(Stream *stream, const void *buffer, u32 count);
	i32 (*seek)(Stream *stream, i32 to, StreamWhence whence);
	i32 (*tell)(Stream *stream);
	i32 (*size)(Stream *stream);
	i32 (*flush)(Stream *stream);
	i32 (*close)(Stream *stream);
} StreamInterface;

static inline i32 Stream_Read(Stream *stream, void *buffer, u32 count);
static inline i32 Stream_Write(Stream *stream, const void *buffer, u32 count);

static inline i32 Stream_Seek(Stream *stream, i32 to, StreamWhence whence);
static inline i32 Stream_Tell(Stream *stream);
static inline i32 Stream_Size(Stream *stream);

static inline i32 Stream_Flush(Stream *stream);
static inline i32 Stream_Close(Stream *stream);

static inline i32 Stream_Read(Stream *stream, void *buffer, u32 count)
{
	StreamInterface *interface = (StreamInterface*)stream;
	if (interface->read == NULL)
		return -1;
	return interface->read(stream, buffer, count);
}

static inline i32 Stream_Write(Stream *stream, const void *buffer, u32 count)
{
	StreamInterface *interface = (StreamInterface*)stream;
	if (interface->write == NULL)
		return -1;
	return interface->write(stream, buffer, count);
}

static inline i32 Stream_Seek(Stream *stream, i32 to, StreamWhence whence)
{
	StreamInterface *interface = (StreamInterface*)stream;
	if (interface->seek == NULL)
		return -1;
	return interface->seek(stream, to, whence);
}

static inline i32 Stream_Tell(Stream *stream)
{
	StreamInterface *interface = (StreamInterface*)stream;
	if (interface->tell == NULL)
		return -1;
	return interface->tell(stream);
}

static inline i32 Stream_Size(Stream *stream)
{
	StreamInterface *interface = (StreamInterface*)stream;
	if (interface->size == NULL)
		return -1;
	return interface->size(stream);
}

static inline i32 Stream_Flush(Stream *stream)
{
	StreamInterface *interface = (StreamInterface*)stream;
	if (interface->flush == NULL)
		return -1;
	return interface->flush(stream);
}

static inline i32 Stream_Close(Stream *stream)
{
	StreamInterface *interface = (StreamInterface*)stream;
	if (interface->close == NULL)
		return -1;
	return interface->close(stream);
}

#endif // HAS_IO_STREAM_H
