#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define CAP_INIT 128 /* initial vector's capacity */

struct ivec {
	int *iv_bufp;
	size_t iv_cap;
	size_t iv_size; /* number of elements in the vector */
};

struct ivec *ivec_init(struct ivec *vp);
int ivec_append(struct ivec *vp, int n);
int ivec_get(struct ivec *vp, ssize_t i, int *n);
void ivec_fini(struct ivec *vp);

int
main(void)
{
	struct ivec v;
	int i, n;

	if (!ivec_init(&v)) {
		printf("ivec_init failed\n");
		return 1;
	}

	for (i = 0; i < 350; ++i)
		if (ivec_append(&v, i) < 0) {
			printf("ivec_append failed\n");
			break;
		}

	while (--i >= 0) {
		ivec_get(&v, i, &n);
		assert(n == i);
	}

	/* invalid index */
	assert(ivec_get(&v, 12345, &n) == -1);
	assert(ivec_get(&v, -1, &n) == -1);

	ivec_fini(&v);

	assert(ivec_append(&v, 7) == -1);

	return 0;
}

struct ivec *
ivec_init(struct ivec *vp)
{
	vp->iv_cap = vp->iv_size = 0;

	vp->iv_bufp = malloc(CAP_INIT * sizeof *vp->iv_bufp);
	if (!vp->iv_bufp)
		return NULL;

	vp->iv_cap = CAP_INIT;

	return vp;
}

int
ivec_append(struct ivec *vp, int n)
{
	size_t newcap;
	int *newbufp;

	if (!vp->iv_cap) /* the vector already deallocated */
		return -1;

	/* ensure capacity */
	if (!(vp->iv_size < vp->iv_cap)) {
		newcap = vp->iv_cap * 2;
		newbufp = realloc(vp->iv_bufp, newcap * sizeof *vp->iv_bufp);
		if (!newbufp)
			return -1;

		vp->iv_cap = newcap;
		vp->iv_bufp = newbufp;
	}

	vp->iv_bufp[vp->iv_size] = n;
	++vp->iv_size;

	return 0;
}

int
ivec_get(struct ivec *vp, ssize_t i, int *n)
{
	if (i < 0 || i >= vp->iv_size)
		return -1;

	*n = vp->iv_bufp[i];

	return 0;
}

void
ivec_fini(struct ivec *vp)
{
	free(vp->iv_bufp);
	vp->iv_bufp = NULL;
	vp->iv_cap = vp->iv_size = 0;
}
