#include <sys/types.h>

#include <assert.h>
#include <string.h>

int
sread(const char *s,
	char *buf, size_t size, off_t offset)
{
	size_t l;
	int n;

	assert(s != NULL);
	assert(buf != NULL);
	assert(offset <= size);

	l = strlen(s);
	if (offset >= l) {
		return 0;
	}

	if (offset + size > l) {
		n = l - offset;
	}

	memcpy(buf, s + offset, n);

	return n;
}

