#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>


// 1. Copy a file (cp)
void copy_file(const char *source, const char *destination) {
    int src = open(source, O_RDONLY);
    if (src == -1) {
        perror("Error opening source file");
        return;
    }
    int dest = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest == -1) {
        perror("Error creating destination file");
        close(src);
        return;
    }
    char buffer[1024];
    ssize_t bytes;
    while ((bytes = read(src, buffer, sizeof(buffer))) > 0) {
        write(dest, buffer, bytes);
    }
    close(src);
    close(dest);
}

// 2. Search for a word in a file (grep)
void grep_word(const char *filename, const char *word) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
    char line[1024];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, word)) {
            found = 1;
            break;
        }
    }
    fclose(file);
    printf("%d\n", found);
}

// 3. List files in a directory (ls)
void list_files(const char *dirname) {
    DIR *dir = opendir(dirname);
    if (!dir) {
        perror("Error opening directory");
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
}



int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: ./program <command> <args>\n");
        return 1;
    }
    if (strcmp(argv[1], "cp") == 0 && argc == 4) {
        copy_file(argv[2], argv[3]);
    } else if (strcmp(argv[1], "grep") == 0 && argc == 4) {
        grep_word(argv[2], argv[3]);
    } else if (strcmp(argv[1], "ls") == 0) {
        list_files(argv[2]);
    } 
   else {
        printf("Invalid command or arguments\n");
    }
    return 0;
}

