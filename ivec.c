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
int ivec_get(struct ivec *vp, int i, int *n);
int ivec_fini(struct ivec *vp);

int
main(void)
{
	struct ivec v;
	int i, n;

	ivec_init(&v);

	for (i = 0; i < 350; ++i)
		if (ivec_append(&v, i) < 0) {
			printf("ivec_append failed\n");
			break;
		}

	while (--i >= 0) {
		ivec_get(&v, i, &n);
		assert(n == i);
	}

	ivec_fini(&v);

	return 0;
}

struct ivec *
ivec_init(struct ivec *vp)
{
	vp->iv_cap = CAP_INIT;
	vp->iv_bufp = malloc(vp->iv_cap * sizeof *vp->iv_bufp);
	vp->iv_size = 0;

	return vp;
}

int
ivec_append(struct ivec *vp, int n)
{
	size_t newcap;
	int *newbufp;

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
ivec_get(struct ivec *vp, int i, int *n)
{
	*n = vp->iv_bufp[i];

	return 0;
}

int
ivec_fini(struct ivec *vp)
{
	free(vp->iv_bufp);

	return 0;
}
