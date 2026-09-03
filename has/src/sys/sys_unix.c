#ifdef __unix__

#include <io/file.h>
#include <unistd.h>

bool File_Exists(const char *filename)
{
	return access(filename, F_OK) == 0;
}

#endif // __unix__
