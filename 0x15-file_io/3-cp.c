#include "main.h"
#include <stdio.h>
#include <stdlib.h>

char *create_buffer(char *file);
void close_file(int fd);

/**
 * create_buffer - Allocates a 1024-byte buffer for file data storage.
 * @file: The name of the file for which the buffer is allocated.
 *
 * Return: A pointer to the newly-allocated buffer.
 */
char *create_buffer(char *file)
{
    char *buffer;

    buffer = malloc(sizeof(char) * 1024);

    if (buffer == NULL)
    {
        dprintf(STDERR_FILENO,
                "Error: Unable to allocate memory for %s\n", file);
        exit(99);
    }

    return (buffer);
}

/**
 * close_file - Closes a file descriptor.
 * @fd: The file descriptor to be closed.
 */
void close_file(int fd)
{
    int result;

    result = close(fd);

    if (result == -1)
    {
        dprintf(STDERR_FILENO, "Error: Unable to close fd %d\n", fd);
        exit(100);
    }
}

/**
 * main - Copies the content of one file to another file.
 * @argc: The number of arguments provided to the program.
 * @argv: An array of pointers to the arguments.
 *
 * Return: 0 on success.
 *
 * Description: Exits with appropriate error codes for various conditions:
 *  - Incorrect argument count: exit code 97.
 *  - Missing or unreadable source file: exit code 98.
 *  - Unwritable target file: exit code 99.
 *  - Failed to close files: exit code 100.
 */
int main(int argc, char *argv[])
{
    int source, target, read_bytes, write_bytes;
    char *buffer;

    if (argc != 3)
    {
        dprintf(STDERR_FILENO, "Usage: cp source_file target_file\n");
        exit(97);
    }

    buffer = create_buffer(argv[2]);
    source = open(argv[1], O_RDONLY);
    read_bytes = read(source, buffer, 1024);
    target = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0664);

    do
    {
        if (source == -1 || read_bytes == -1)
        {
            dprintf(STDERR_FILENO,
                    "Error: Unable to read from file %s\n", argv[1]);
            free(buffer);
            exit(98);
        }

        write_bytes = write(target, buffer, read_bytes);
        if (target == -1 || write_bytes == -1)
        {
            dprintf(STDERR_FILENO,
                    "Error: Unable to write to %s\n", argv[2]);
            free(buffer);
            exit(99);
        }

        read_bytes = read(source, buffer, 1024);
        target = open(argv[2], O_WRONLY | O_APPEND);

    } while (read_bytes > 0);

    free(buffer);
    close_file(source);
    close_file(target);

    return (0);
}
