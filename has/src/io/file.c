#include "file.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	StreamInterface interface;
	FILE *file;
} FileStream;

Stream *File_Open(const char *filename, StreamMode mode)
{
	const char *m;
	switch (mode) {
	case STREAM_MODE_R:
		m = "r";
		break;
	case STREAM_MODE_W:
		m = "w";
		break;
	case STREAM_MODE_RW:
		m = "rw";
		break;
	case STREAM_MODE_RB:
		m = "rb";
		break;
	case STREAM_MODE_WB:
		m = "wb";
		break;
	case STREAM_MODE_RWB:
		m = "rwb";
		break;
	default:
		return NULL;
	}

	FILE *f = fopen(filename, m);
	if (f == NULL)
		return NULL;

	FileStream result = { 0 };
	switch (mode) {
	case STREAM_MODE_R:
	case STREAM_MODE_RB:
		result.interface.read = File_Read;
		break;
	case STREAM_MODE_W:
	case STREAM_MODE_WB:
		result.interface.write = File_Write;
		result.interface.flush = File_Flush;
		break;
	case STREAM_MODE_RW:
	case STREAM_MODE_RWB:
		result.interface.read = File_Read;
		result.interface.write = File_Write;
		result.interface.flush = File_Flush;
		break;
	default:
		break;
	}
	result.interface.seek = File_Seek;
	result.interface.tell = File_Tell;
	result.interface.size = File_Size;
	result.interface.close = File_Close;
	result.file = f;

	FileStream *stream = malloc(sizeof(FileStream));
	*stream = result;
	return (Stream*)stream;
}

i32 File_Read(Stream *stream, void *buffer, u32 count)
{
	FileStream *fileStream = (FileStream*)stream;
	return (i32)fread(buffer, count, 1, fileStream->file);
}

i32 File_Write(Stream *stream, const void *buffer, u32 count)
{
	FileStream *fileStream = (FileStream*)stream;
	return (i32)fwrite(buffer, count, 1, fileStream->file);
}

i32 File_Seek(Stream *stream, i32 to, StreamWhence whence)
{
	FileStream *fileStream = (FileStream*)stream;
	return (i32)fseek(fileStream->file, to, whence);
}

i32 File_Tell(Stream *stream)
{
	FileStream *fileStream = (FileStream*)stream;
	return (i32)ftell(fileStream->file);
}

i32 File_Size(Stream *stream)
{
	FileStream *fileStream = (FileStream*)stream;
	i32 p = (i32)ftell(fileStream->file);
	fseek(fileStream->file, 0, SEEK_END);
	i32 result = (i32)ftell(fileStream->file);
	fseek(fileStream->file, p, SEEK_SET);
	return result;
}

i32 File_Flush(Stream *stream)
{
	FileStream *fileStream = (FileStream*)stream;
	return fflush(fileStream->file);
}

i32 File_Close(Stream *stream)
{
	FileStream *fileStream = (FileStream*)stream;
	i32 result = fclose(fileStream->file);
	free(fileStream);
	return result;
}
