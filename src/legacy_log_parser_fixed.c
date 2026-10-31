#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 128
#define MAX_INPUT (BUF_SIZE - 1)

__attribute__((noinline))
static int safe_copy(const unsigned char *input, size_t n)
{
    char buf[BUF_SIZE];

   
    if (n > MAX_INPUT) {
        fprintf(stderr,
                "Error: input exceeds the %d-byte limit.\n",
                MAX_INPUT);
        return EXIT_FAILURE;
    }

    memcpy(buf, input, n);
    buf[n] = '\0';

    printf("Log preview: %.64s\n", buf);
    return EXIT_SUCCESS;
}

static int parse_log(const char *path)
{
    FILE *file = fopen(path, "rb");

    if (file == NULL) {
        perror("Failed to open log file");
        return EXIT_FAILURE;
    }

   
    unsigned char input[MAX_INPUT + 1];
    size_t n = fread(input, 1, sizeof(input), file);

    if (ferror(file)) {
        fprintf(stderr, "Error: failed to read log file.\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    if (fclose(file) != 0) {
        perror("Failed to close log file");
        return EXIT_FAILURE;
    }

    return safe_copy(input, n);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <log_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    return parse_log(argv[1]);
}
