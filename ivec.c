#include <assert.h>
#include <stdlib.h>

struct ivec {
	int *iv_bufp;
	int iv_cap;
	int iv_size; /* number of elements in the vector */
};

void ivec_init(struct ivec *vp);
void ivec_append(struct ivec *vp, int n);
int ivec_get(struct ivec *vp, int i);
void ivec_fini(struct ivec *vp);

int
main(void)
{
	struct ivec v;
	int i, n;

	ivec_init(&v);

	for (i = 0; i < 10; ++i)
		ivec_append(&v, i);

	for (i = 0; i < 10; ++i) {
		n = ivec_get(&v, i);
		assert(n == i);
	}

	ivec_fini(&v);

	return 0;
}

void
ivec_init(struct ivec *vp)
{
	vp->iv_cap = 128;
	vp->iv_bufp = malloc(vp->iv_cap * sizeof *vp->iv_bufp);
	vp->iv_size = 0;
}

void
ivec_append(struct ivec *vp, int n)
{
	if (!(vp->iv_size < vp->iv_cap)) {
		vp->iv_cap *= 2;
		vp->iv_bufp = realloc(vp->iv_bufp, vp->iv_cap * *vp->iv_bufp);
	}

	vp->iv_bufp[vp->iv_size] = n;
	++vp->iv_size;
}

int
ivec_get(struct ivec *vp, int i)
{
	return vp->iv_bufp[i];
}

void
ivec_fini(struct ivec *vp)
{
	free(vp->iv_bufp);
}
