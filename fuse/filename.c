#include <assert.h>
#include <string.h>

const char *
filename(const char *path)
{
	const char *p;

	assert(path != NULL);

	p = strrchr(path, '/');
	if (p == NULL) {
		return path;
	}

	return p + 1;
}

