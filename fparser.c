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
        char *path = strtok(NULL, " ");
        char *fileName = strtok(NULL, " ");
        if (path != NULL && fileName != NULL)
        {
            struct File *file = getFileInDirectory(fs, path, fileName);
            if (file != NULL && file->access <= fs->current_user.access_level)
            {
                displayFileInDirectory(fs, path, fileName);
                return;
            }
            else
            {
                printf("Insufficient permissions to display the file '%s' in the directory '%s'.\n", fileName, path);
                return;
            }
        }
    }

    if (strcmp(cmd, "rf") == 0)
    {
        char *filePath = strtok(NULL, " ");
        char *fileName = strtok(NULL, " ");

        if (filePath != NULL && fileName != NULL)
        {
            struct File *file = goTo(fs, filePath);
            if (file != NULL && strcmp(file->name, fileName) == 0 && file->access <= fs->current_user.access_level)
            {
                readFile(fs, filePath, fileName);
                return;
            }
            else
            {
                printf("Insufficient permissions to read the file '%s' at path '%s'.\n", fileName, filePath);
                return;
            }
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
                struct File *file = getFileInDirectory(fs, filePath, fileName);

                if (file != NULL)
                {
                    // Check authority level before writing
                    if (fs->current_user.access_level >= file->access)
                    {
                        writeFile(fs, filePath, fileName, content);
                        return;
                    }
                    else
                    {
                        printf("Insufficient permissions to write to file '%s'.\n", fileName);
                        return;
                    }
                }
                else
                {
                    printf("File not found at path: %s/%s\n", filePath, fileName);
                    return;
                }
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
                struct File *file = getFileInDirectory(fs, sourcePath, fileName);

                if (file != NULL)
                {
                    // Check authority level before moving the file
                    if (fs->current_user.access_level >= file->access)
                    {
                        moveFileAtPath(fs, sourcePath, destinationPath, fileName);
                        return;
                    }
                    else
                    {
                        printf("Insufficient permissions to move the file '%s'.\n", fileName);
                        return;
                    }
                }
                else
                {
                    printf("File not found at path: %s/%s\n", sourcePath, fileName);
                    return;
                }
            }
        }

        if (strcmp(cmd, "chal") == 0)
        {
            char *flag = strtok(NULL, " ");
            if (flag != NULL)
            {
                if (strcmp(flag, "-d") == 0 || strcmp(flag, "-f") == 0)
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
                    // Add more conditions as needed for different access levels

                    if (strcmp(flag, "-f") == 0)
                    {
                        fileName = strtok(NULL, " ");

                        if (fileName == NULL)
                        {
                            printf("Please provide the filename.\n");
                            return;
                        }
                    }

                    if (strcmp(flag, "-f") == 0 && fileName != NULL)
                    {
                        changeFileAccessLevel(fs, path, fileName, newAccessLevel);
                    }
                    else if (strcmp(flag, "-d") == 0)
                    {
                        changeDirectoryAccessLevel(fs, path, newAccessLevel);
                    }
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
    }

    printf("Invalid command or insufficient permissions for command '%s' with parameters.\n", cmd);
    printf("Current user level: %d\n", currentUserLevel);
}
