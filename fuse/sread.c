#include <sys/types.h>

#include <assert.h>
#include <string.h>

int
sread(const char *s,
	char *buf, size_t size, size_t offset)
{
	size_t l;

	assert(s != NULL);
	assert(buf != NULL);
	assert(offset <= size);

	l = strlen(s);
	if (offset >= l) {
		return 0;
	}

	if (offset + size > l) {
		size = l - offset;
	}

	memcpy(buf, s + offset, size);

	return size;
}

