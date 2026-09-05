#include "path.h"

u32 Path_Normalize(char *buffer, u32 n, const char *str, u32 strn)
{
	if (str == NULL || strn == 0)
		return 0;

	u32 length = 0;
	u32 bufferPos = 0;
	char last = '\0';
	for (u32 i = 0; i < strn && str[i] != '\0'; ++i) {
		char cur = str[i] == '\\' ? '/' : str[i];
		if (cur == '/' && last == '/')
			continue;
		if (buffer != NULL && n > 0 && bufferPos < n - 1)
			buffer[bufferPos++] = cur;
		++length;
		last = cur;
	}
	if (buffer != NULL && n > 0)
		buffer[bufferPos] = '\0';

	return length;
}

u32 Path_Join(char *buffer, u32 n, const char *a, u32 an, const char *b, u32 bn)
{
	u32 length = 0;
	u32 bufferPos = 0;
	char last = '\0';

	if (a != NULL && an > 0) {
		for (u32 i = 0; i < an && a[i] != '\0'; ++i) {
			char cur = a[i] == '\\' ? '/' : a[i];
			if (cur == '/' && last == '/')
				continue;
			if (buffer != NULL && n > 0 && bufferPos < n - 1)
				buffer[bufferPos++] = cur;
			++length;
			last = cur;
		}
	}
	if (b != NULL && bn > 0) {
		if (last != '/' && last != '\0') {
			if (buffer != NULL && n > 0 && bufferPos < n - 1)
				buffer[bufferPos++] = '/';
			++length;
			last = '/';
		}
		for (u32 i = 0; i < bn && b[i] != '\0'; ++i) {
			char cur = b[i] == '\\' ? '/' : b[i];
			if (cur == '/' && last == '/')
				continue;
			if (buffer != NULL && n > 0 && bufferPos < n - 1)
				buffer[bufferPos++] = cur;
			++length;
			last = cur;
		}
	}
	if (buffer != NULL && n > 0)
		buffer[bufferPos] = '\0';

	return length;
}
