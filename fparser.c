#include "fsys.h"

void parseCommand(struct FileSystem *fs, const char *command)
{
    enum AuthorityLevel currentUserLevel = fs->current_user.access_level;

    char *cmd = strtok(command, " ");

    if (strcmp(cmd, "goto") == 0)
    {
        char *path = strtok(NULL, " ");
        if (path != NULL)
        {
            goTo(fs, path);
            return;
        }
    }

    if (strcmp(cmd, "find") == 0)
    {
        char *path = strtok(NULL, " ");
        char *fileName = strtok(NULL, " ");
        if (path != NULL && fileName != NULL)
        {
            searchFileInPath(fs, path, fileName);
            return;
        }
    }

    if (strcmp(cmd, "dispd") == 0)
    {
        char *path = strtok(NULL, " ");
        if (path != NULL)
        {
            displayCurrentDirectory(fs, path);
            return;
        }
    }

    if (strcmp(cmd, "dispf") == 0)
    {
        const char *path = strtok(NULL, " ");
        const char *fileName = strtok(NULL, " ");
        if (path != NULL && fileName != NULL)
        {
            displayFileInDirectory(fs, path, fileName);
            return;
        }
    }

    if (strcmp(cmd, "rf") == 0)
    {
        char *filePath = strtok(NULL, " ");
        char *fileName = strtok(NULL, " ");

        if (filePath != NULL && fileName != NULL)
        {
            readFile(fs, filePath, fileName);
            return;
        }
    }

    if (strcmp(cmd, "load") == 0)
    {
        char *fileName = strtok(NULL, " ");
        char *windowsPath = strtok(NULL, " ");
        char *subsystemPath = strtok(NULL, " ");

        char *fileNameCopy = (fileName != NULL) ? strdup(fileName) : NULL;
        char *windowsPathCopy = (windowsPath != NULL) ? strdup(windowsPath) : NULL;
        char *subsystemPathCopy = (subsystemPath != NULL) ? strdup(subsystemPath) : NULL;

        if (fileNameCopy != NULL && windowsPathCopy != NULL && subsystemPathCopy != NULL)
        {
            int result = loadFileContent(fileName, windowsPath, subsystemPath, fs);
            if (result == 0)
            {
                printf("File '%s' loaded into subsystem file '%s/%s' .\n", windowsPathCopy, subsystemPathCopy, fileNameCopy);
                free(fileNameCopy);
                free(windowsPathCopy);
                free(subsystemPathCopy);
                return;
            }
            else
            {
                printf("Failed to load file into subsystem '%s/%s'.\n", subsystemPathCopy, fileNameCopy);
                free(fileNameCopy);
                free(windowsPathCopy);
                free(subsystemPathCopy);
                return;
            }
        }

        // Free memory for copied variables if they are not NULL
        if (fileNameCopy != NULL)
        {
            free(fileNameCopy);
        }
        if (windowsPathCopy != NULL)
        {
            free(windowsPathCopy);
        }
        if (subsystemPathCopy != NULL)
        {
            free(subsystemPathCopy);
        }
    }

    if (strcmp(cmd, "output") == 0)
    {
        char *fileName = strtok(NULL, " ");
        char *subsystemPath = strtok(NULL, " ");
        char *windowsPath = strtok(NULL, " ");

        char *fileNameCopy = (fileName != NULL) ? strdup(fileName) : NULL;
        char *subsystemPathCopy = (subsystemPath != NULL) ? strdup(subsystemPath) : NULL;
        char *windowsPathCopy = (windowsPath != NULL) ? strdup(windowsPath) : NULL;

        if (fileNameCopy != NULL && windowsPathCopy != NULL && subsystemPathCopy != NULL)
        {
            int result = outFileContent(fileName, subsystemPath, windowsPath, fs);
            if (result == 0)
            {
                printf("File content successfully written to Windows file '%s' from subsystem '%s/%s'.\n", windowsPathCopy, subsystemPathCopy, fileNameCopy);
                free(fileNameCopy);
                free(windowsPathCopy);
                free(subsystemPathCopy);
                return;
            }
            else
            {
                printf("Failed to write file content to Windows file '%s' from subsystem '%s/%s'.\n", windowsPathCopy, subsystemPathCopy, fileNameCopy);
                free(fileNameCopy);
                free(windowsPathCopy);
                free(subsystemPathCopy);
                return;
            }
        }
        else
        {
            printf("Invalid parameters provided for 'output'.\n");
            // Free allocated memory for copies if necessary
            return;
        }
    }

    if (strcmp(cmd, "login") == 0)
    {
        char *username = strtok(NULL, " ");
        char *password = strtok(NULL, " ");

        if (username != NULL && password != NULL)
        {
            loginUser(fs, username, password);
            return;
        }
    }

    if (strcmp(cmd, "reset") == 0)
    {
        char *username = strtok(NULL, " ");
        char *oldPassword = strtok(NULL, " ");
        char *newPassword = strtok(NULL, " ");

        if (username != NULL && oldPassword != NULL && newPassword != NULL)
        {
            resetPassword(fs, username, oldPassword, newPassword);
            return;
        }
    }

    if (strcmp(cmd, "d") == 0)
    {
        char *path = strtok(NULL, " ");
        char *name = strtok(NULL, " ");
        if (path != NULL && name != NULL)
        {
            createDirectory(fs, path, name);
            return;
        }
    }

    if (strcmp(cmd, "f") == 0)
    {
        char *path = strtok(NULL, " ");
        char *name = strtok(NULL, " ");
        if (path != NULL && name != NULL)
        {
            createFileInDir(fs, path, name);
            return;
        }
    }

    if (currentUserLevel >= MED)
    {
        if (strcmp(cmd, "wf") == 0)
        {
            char *filePath = strtok(NULL, " ");
            char *fileName = strtok(NULL, " ");
            char *content = strtok(NULL, "\n");

            if (filePath != NULL && fileName != NULL && content != NULL)
            {
                writeFile(fs, filePath, fileName, content);
                return;
            }
            else
            {
                printf("File not found at path: %s/%s\n", filePath, fileName);
                return;
            }
        }
    }

    if (currentUserLevel >= HIGH)
    {
        if (strcmp(cmd, "md") == 0)
        {
            char *sourcePath = strtok(NULL, " ");
            char *destinationPath = strtok(NULL, " ");
            if (sourcePath != NULL && destinationPath != NULL)
            {
                struct Directory *sourceDir = goTo(fs, sourcePath);
                struct Directory *destinationDir = goTo(fs, destinationPath);

                if (sourceDir != NULL && destinationDir != NULL)
                {
                    // Check authority level before moving the directory
                    if (fs->current_user.access_level >= sourceDir->access && fs->current_user.access_level >= destinationDir->access)
                    {
                        moveDirectoryAtPath(fs, sourcePath, destinationPath);
                        return;
                    }
                    else
                    {
                        printf("Insufficient permissions to move the directory.\n");
                        return;
                    }
                }
                else
                {
                    printf("Invalid source or destination directory. Cannot move.\n");
                    return;
                }
            }
        }

        if (strcmp(cmd, "mf") == 0)
        {
            char *sourcePath = strtok(NULL, " ");
            char *destinationPath = strtok(NULL, " ");
            char *fileName = strtok(NULL, " ");
            if (sourcePath != NULL && destinationPath != NULL && fileName != NULL)
            {
                moveFileAtPath(fs, sourcePath, destinationPath, fileName);
                return;
            }
            else
            {
                printf("File not found at path: %s/%s\n", sourcePath, fileName);
                return;
            }
        }

        if (strcmp(cmd, "dd") == 0)
        {
            char *path = strtok(NULL, " ");
            if (path != NULL)
            {
                deleteDirectoryAtPath(fs, path);
                return;
            }
        }

        if (strcmp(cmd, "df") == 0)
        {
            char *path = strtok(NULL, " ");
            char *fileName = strtok(NULL, " ");
            if (path != NULL && fileName != NULL)
            {
                deleteFileAtPath(fs, path, fileName);
                return;
            }
        }

        if (strcmp(cmd, "chal") == 0)
        {
            char *flag = strtok(NULL, " ");
            if (flag != NULL)
            {
                if (strcmp(flag, "-d") == 0)
                {
                    char *path = strtok(NULL, " ");
                    char *fileName = NULL;
                    char *accessStr = strtok(NULL, " "); // Access level input by user

                    if (path == NULL || accessStr == NULL)
                    {
                        printf("Please provide the path and new access level.\n");
                        return;
                    }

                    enum AuthorityLevel newAccessLevel = LOW; // Default access level
                    if (strcmp(accessStr, "MED") == 0)
                    {
                        newAccessLevel = MED;
                    }
                    else if (strcmp(accessStr, "HIGH") == 0)
                    {
                        newAccessLevel = HIGH;
                    }

                    changeDirectoryAccessLevel(fs, path, newAccessLevel);
                    return;
                }
            }
        }
    }

    if (currentUserLevel >= HIGHEST)
    {
        if (strcmp(cmd, "addUser") == 0)
        {
            char *username = strtok(NULL, " ");
            char *password = strtok(NULL, " ");
            char *level = strtok(NULL, " ");

            if (username != NULL && password != NULL && level != NULL)
            {
                int accessLevel = atoi(level);
                addUserToSystem(fs, username, password, accessLevel);
                return;
            }
        }

        if (strcmp(cmd, "delUser") == 0)
        {
            char *username = strtok(NULL, " ");
            if (username != NULL)
            {
                deleteUserFromSystem(fs, username);
                return;
            }
        }
    }

    printf("Invalid command or insufficient permissions for command '%s' with parameters.\n", cmd);
    printf("Current user level: %d\n", currentUserLevel);
}
