
#include "Manager.h"

int main(int argc, char **argv)
{
    if (argc > 1) {
	for (int i = strlen(argv[0])-1; i > 0 && argv[0][i] != '/'; --i);
	fprintf(stderr, "usage: %s\n", argv[0] + (i > 0) + i);
	exit(2);
    }

    WindowManager manager;
    return 0;
}

