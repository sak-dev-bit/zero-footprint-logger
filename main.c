#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UPTIME_PATH "/proc/uptime"

int main(int argc, char *argv[]) {
    FILE *fp;
    char buffer[64];
    double uptime_seconds;

    // 1. Open the low-level Linux system file
    fp = fopen(UPTIME_PATH, "r");
    if (fp == NULL) {
        perror("Error: Failed to open /proc/uptime");
        return 1;
    }

    // 2. Read the first value (uptime in seconds)
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        sscanf(buffer, "%lf", &uptime_seconds);
    } else {
        fprintf(stderr, "Error: Could not read from /proc/uptime\n");
        fclose(fp);
        return 1;
    }

    fclose(fp);

    // 3. Print the output to stdout (can be captured by the container runtime)
    printf("System Uptime: %.2f seconds\n", uptime_seconds);

    return 0; // Exit immediately after logging
}