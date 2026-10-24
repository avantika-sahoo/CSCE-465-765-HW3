#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 128
#define MAX_COPY 512

__attribute__((noinline))
void win(void) {
    FILE *marker = fopen("markers/ret2win.txt", "w");

    if (marker != NULL) {
        fprintf(marker, "win() was executed.\n");
        fclose(marker);
        printf("[WIN] Marker file created.\n");
    } else {
        perror("[WIN] Failed to create marker");
    }

    FILE *decoy = fopen("/opt/agentsec/decoy.txt", "r");

    if (decoy != NULL) {
        char line[128];

        if (fgets(line, sizeof(line), decoy) != NULL) {
            printf("[WIN] Decoy contents: %s", line);
        }

        fclose(decoy);
    } else {
        perror("[WIN] Failed to read decoy");
    }
}

__attribute__((noinline))
static void vulnerable_copy(const unsigned char *input, size_t n) {
    char buf[BUF_SIZE];

    memcpy(buf, input, n);

    printf("Log preview: %.64s\n", buf);
}

static void parse_log(const char *path) {
    FILE *file = fopen(path, "rb");

    if (file == NULL) {
        perror("Failed to open log file");
        return;
    }

    unsigned char *input = malloc(MAX_COPY);

    if (input == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return;
    }

    size_t n = fread(input, 1, MAX_COPY, file);

    if (ferror(file)) {
        perror("Failed to read log file");
        free(input);
        fclose(file);
        return;
    }

    fclose(file);

    vulnerable_copy(input, n);

    free(input);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <log_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    parse_log(argv[1]);

    return EXIT_SUCCESS;
}
