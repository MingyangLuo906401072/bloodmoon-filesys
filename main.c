#include "fparser.h"

#define MAX_COMMAND_LENGTH 100

int main()
{
    struct FileSystem fs;
    initFileSystem(&fs);

    char command[MAX_COMMAND_LENGTH];

    printf("File System Operations. Type 'exit' or 'logout' to quit.\n");
    while (1)
    {
        char *crr = getCurrentDirectoryPath(&fs);
        printf("%s>> ", crr);

        free(crr);

        fgets(command, MAX_COMMAND_LENGTH, stdin);
        command[strcspn(command, "\n")] = '\0';

        int is_only_spaces = 1;
        for (int i = 0; command[i] != '\0'; ++i)
        {
            if (!isspace((unsigned char)command[i]))
            {
                is_only_spaces = 0;
                break;
            }
        }

        if (command[0] == '\0' || is_only_spaces)
        {
            continue;
        }

        if (strcmp(command, "exit") == 0 || strcmp(command, "logout") == 0)
        {
            printf("Exiting...\n");
            break;
        }

        parseCommand(&fs, command);
    }

    return 0;
}