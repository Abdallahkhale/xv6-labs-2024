#include "kernel/types.h"
#include "user/user.h"


int main() {
    int fd[2]; // File descriptors for the pipe: fd[0] for reading, fd[1] for writing
    int pid;
    char message[] = "Hello from parent!";
    char buffer[100];

    // Create a pipe
    pipe(fd);

    // Fork a child process
    pid = fork();

  

    if (pid > 0) { // Parent process
        close(fd[0]); // Close reading end
        write(fd[1], message, strlen(message)+1); // Write message to pipe
        close(fd[1]); // Close writing end after writing
    } else { // Child process
        close(fd[1]); // Close writing end
        read(fd[0], buffer, sizeof(buffer)); // Read message from pipe
        printf("Child received: %s\n", buffer);
        close(fd[0]); // Close reading end
    }

    exit(0);
}

