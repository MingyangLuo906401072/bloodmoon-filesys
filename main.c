#include "fparser.h"

#define MAX_COMMAND_LENGTH 100

int main()
{
    struct FileSystem fs;
    initFileSystem(&fs);

    char command[MAX_COMMAND_LENGTH];

    time_t currentTime;
    struct tm *currentLocalTime;

    currentTime = time(NULL);
    currentLocalTime = localtime(&currentTime);

    printf("Bloodmoon File System. %02d-%02d-%02d %02d:%02d\n",
           currentLocalTime->tm_year % 100, currentLocalTime->tm_mon + 1,
           currentLocalTime->tm_mday, currentLocalTime->tm_hour, currentLocalTime->tm_min);

    printf("\033[0;31m");
    printf("       /  |             |  \\\n");
    printf("      /               |     \\\n");
    printf("     /    @@@   |   @@@   \\\n");
    printf("    /  @@@@@  |  @@@@@  \\\n");
    printf("   /  @@@@@@@| @@@@@@@  \\\n");
    printf("  | @@@@@@@@@| @@@@@@@@@ |\n");
    printf(" |  @@@@@@@@@@@@@@@    / |\n");
    printf(" |  @@@@@@@@@@@@@@@   /| |\n");
    printf(" |  @@@@@@@@@@@@@@@   | |\n");
    printf("  |  @@@@@@@@@@@@@  | |\n");
    printf("   \\ @@@@@@@@@@@  / /\n");
    printf("    \\ @@@@@@@  / /\n");
    printf("     \\    |      |  //\n");
    printf("       \\ | | | | /\n");
    printf("          | | | |\n");
    printf("\033[0m");

    while (1)
    {
        time_t currentTime = time(NULL);
        struct tm *currentLocalTime = localtime(&currentTime);

        char *currentUser = getCurrentUser(&fs);
        char *currentDirPath = getCurrentDirectoryPath(&fs);

        printf("\033[0;35m");
        printf("%04d-%02d-%02d %02d:%02d ",
               currentLocalTime->tm_year + 1900, currentLocalTime->tm_mon + 1,
               currentLocalTime->tm_mday, currentLocalTime->tm_hour,
               currentLocalTime->tm_min);
        printf("\033[0;32m%s\033[0m", currentUser);
        printf("\033[0;31m%s\033[0m", "@bloodmoon:");
        printf("\033[0;31m%s\033[0m", ":");
        printf("\033[0;32m%s\033[0m", currentDirPath);
        printf("\033[0;31m%s\033[0m", ">> ");

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
