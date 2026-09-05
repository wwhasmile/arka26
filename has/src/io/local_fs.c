#include "local_fs.h"
#include <io/file.h>
#include <str/path.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	FsInterface interface;
	u32 rootLength;
	char root[];
} LocalFs;

Fs *LocalFs_Init(const char *root)
{
	FsInterface interface = {
		LocalFs_Exists,
		LocalFs_Open,
		LocalFs_FileSize,
		LocalFs_Release,
	};
	u32 rootLength = Path_Normalize(NULL, 0, root, PATH_MAX);
	LocalFs *result = malloc(sizeof(LocalFs) + rootLength + 1);
	result->interface = interface;
	result->rootLength = rootLength;
	Path_Normalize(result->root, rootLength + 1, root, PATH_MAX);
	
	return (Fs*)result;
}

bool LocalFs_Exists(Fs *fs, const char *file)
{
	LocalFs *it = (LocalFs*)fs;

	char buffer[PATH_MAX + 1];
	u32 resultLength = Path_Join(buffer, PATH_MAX + 1, it->root, it->rootLength,
				  file, PATH_MAX - it->rootLength);
	if (resultLength > PATH_MAX)
		return false;

	return File_Exists(buffer);
}

Stream *LocalFs_Open(Fs *fs, const char *file, StreamMode mode)
{
	LocalFs *it = (LocalFs*)fs;

	char buffer[PATH_MAX + 1];
	u32 resultLength = Path_Join(buffer, PATH_MAX + 1, it->root, it->rootLength,file, PATH_MAX);
	if (resultLength > PATH_MAX)
		return NULL;

	return File_Open(buffer, mode);
}

i32 LocalFs_Release(Fs *fs)
{
	free(fs);
	return 0;
}
