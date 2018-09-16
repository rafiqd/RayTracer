#include "perlin.h"


static Vector3f* perlin_generate() {
    Vector3f * p = new Vector3f[256];
    for ( int i = 0; i < 256; ++i )
        p[i] = unit_vector(Vector3f(-1 + 2*drand48(), -1 + 2*drand48(), -1 + 2*drand48()));
    return p;
}

void permute(int *p, int n) {
    for (int i = n-1; i > 0; i--) {
        int target = int(drand48()*(i+1));
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
    return;
}

static int* perlin_generate_perm() {
    int * p = new int[256];
    for (int i = 0; i < 256; i++)
        p[i] = i;
    permute(p, 256);
    return p;
}

Vector3f *perlin::ranvec = perlin_generate();
int *perlin::perm_x = perlin_generate_perm();
int *perlin::perm_y = perlin_generate_perm();
int *perlin::perm_z = perlin_generate_perm();