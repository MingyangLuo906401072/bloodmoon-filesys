#include "fparser.h"
#include <time.h>

#define MAX_COMMAND_LENGTH 100

int main()
{
    struct FileSystem fs;
    initFileSystem(&fs);

    char command[MAX_COMMAND_LENGTH];

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    printf("Bloodmoon File System. %02d-%02d-%02d %02d:%02d\n", tm.tm_year % 100,
           tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min);
    while (1)
    {
        char *currentUser = getCurrentUser(&fs);
        char *currentDirPath = getCurrentDirectoryPath(&fs);
        printf("%s@bloodmoon:%s>> ", currentUser, currentDirPath);

        free(currentUser);
        free(currentDirPath);

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
