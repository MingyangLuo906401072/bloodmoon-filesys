#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define MAX_FILES 100
#define MAX_CONTENT_SIZE 1000
#define MAX_DIRS 50
#define MAX_PATH_LENGTH 500
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_USERS 50
#define MAX_SUB_DIRS 50
#define MAX_FILE_NAME_LENGTH 255
#define MAX_CHARS 255

enum AuthorityLevel 
{
    LOW,
    MED,
    HIGH,
    HIGHEST
};

struct User 
{
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    enum AuthorityLevel access_level;
};

struct File {
    char name[MAX_FILE_NAME_LENGTH];
    char path[MAX_PATH_LENGTH];
    HANDLE hMapFile;  // Handle to the shared memory map
    LPVOID fileContent; // Pointer to the shared memory content
    int size;
};

struct Directory 
{
    char name[MAX_FILE_NAME_LENGTH];
    char path[MAX_PATH_LENGTH];
    struct File *files[MAX_FILES];
    int file_count;
    struct Directory *subdirectories[MAX_SUB_DIRS];
    int subdir_count;
    enum AuthorityLevel access; 
};

struct FileSystem {
    struct Directory *root;
    struct Directory *current_directory;
    struct User users[MAX_USERS];
    int user_count;
    struct User current_user;
};

void addUserToSystem(struct FileSystem *fs, const char *username, const char *password, enum AuthorityLevel accessLevel);

void deleteUserFromSystem(struct FileSystem *fs, const char *username);

struct User loginUser(struct FileSystem *fs, const char *username, const char *password);

void resetPassword(struct FileSystem *fs, const char *username, const char *oldPassword, const char *newPassword);

void initUser(struct User *user);

void initFile(struct File *file);

void initDirectory(struct Directory *dir);

void initFileSystem(struct FileSystem *fs);

void createDirectory(struct FileSystem *fs, const char *path, const char *name);

void createFileInDir(struct FileSystem *fs, const char *path, const char *name);

void writeFile(struct FileSystem *fs, const char *filePath, const char *fileName, const char *content);

void readFile(struct FileSystem *fs, const char *filePath, const char *fileName);

void deleteFileAtPath(struct FileSystem *fs, const char *path, const char *fileName);

void deleteDirectoryAtPath(struct FileSystem *fs, const char *path);

void moveDirectoryAtPath(struct FileSystem *fs, const char *sourcePath, const char *destinationPath);

void moveFileAtPath(struct FileSystem *fs, const char *sourcePath, const char *destinationPath, const char *fileName);

void searchFileInPath(struct FileSystem *fs, const char *path, const char *fileName);

struct Directory *goTo(struct FileSystem *fs, const char *path);

void displayCurrentDirectory(struct FileSystem *fs, const char *path);

void displayFileInDirectory(struct FileSystem *fs, const char *path, const char *fileName);

char *getCurrentDirectoryPath(struct FileSystem *fs);

int loadFileContent(const char *fileName, const char *windowsPath, const char *subsystemPath, struct FileSystem *fs);

void changeFileAccessLevel(struct FileSystem *fs, const char *path, const char *fileName, enum AuthorityLevel newAccessLevel);

void changeDirectoryAccessLevel(struct FileSystem *fs, const char *dirPath, enum AuthorityLevel newAccessLevel);

struct File *getFileInDirectory(struct FileSystem *fs, const char *path, const char *fileName);

char *getCurrentUser(struct FileSystem *fs);

#endif /* FILESYSTEM_H */