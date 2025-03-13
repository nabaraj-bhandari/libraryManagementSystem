// database.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "headers.h"
//=================================================================================================

#define USER_FILE "users.dat"
#define BACKUP_DIR "backup/"
//=================================================================================================

int validate_string(const char *str, int max_len) {
    int len = strlen(str);
    return (len > 0 && len < max_len && strchr(str, ' ') == NULL);
}
//=================================================================================================

void database_init() {
    FILE *fp = fopen(USER_FILE, "rb");
    if (!fp) {
        fp = fopen(USER_FILE, "wb");
        if (!fp) {
            printf("Error creating users file!\n");
            return;
        }

        User admin = {"admin", "admin123", "Administrator", "Admin", 0, 1};
        fwrite(&admin, sizeof(User), 1, fp);
        fclose(fp);
        printf("Initialized database with default admin user\n");
    } else {
        fclose(fp);
    }
}
//=================================================================================================

int authenticate_user(char *username, char *password, User *user) {  // Updated function
    if (!validate_string(username, MAX_USERNAME) ||
        !validate_string(password, MAX_PASSWORD)) {
        return 0;
    }

    FILE *fp = fopen(USER_FILE, "rb+"); // rb+ for read/write
    if (!fp) {
        return 0;
    }

    User temp;
    long pos;
    while (fread(&temp, sizeof(User), 1, fp)) {
        if (strcmp(temp.username, username) == 0) {
            pos = ftell(fp) - sizeof(User);
            if (temp.failed_attempts >= 3) {
                printf("Account locked due to multiple failed attempts.\n");
                fclose(fp);
                return 0;
            }
            if (strcmp(temp.password, password) == 0) {
                temp.failed_attempts = 0;
                fseek(fp, pos, SEEK_SET);
                fwrite(&temp, sizeof(User), 1, fp);
                *user = temp;
                fclose(fp);
                return 1;
            } else {
                temp.failed_attempts++;
                printf("Incorrect password. Failed attempts: %d\n", temp.failed_attempts);
                fseek(fp, pos, SEEK_SET);
                fwrite(&temp, sizeof(User), 1, fp);
                fclose(fp);
                return 0;
            }
        }
    }

    fclose(fp);
    printf("User not found.\n");
    return 0;
}
//=================================================================================================

int create_user(char *username, char *password, char *name, int is_admin) {
    if (!validate_string(username, MAX_USERNAME) ||
        !validate_string(password, MAX_PASSWORD)) {
        printf("Invalid input: Username and password must be non-empty, less than max length, and contain no spaces.\n");
        return 0;
    }

    FILE *fp = fopen(USER_FILE, "rb");
    if (fp) {
        User temp;
        while (fread(&temp, sizeof(User), 1, fp)) {
            if (strcmp(temp.username, username) == 0) {
                fclose(fp);
                printf("Username already exists!\n");
                return 0;
            }
        }
        fclose(fp);
    }

    fp = fopen(USER_FILE, "ab");
    if (!fp) {
        printf("Error opening user file for writing!\n");
        return 0;
    }

    User new_user;
    strncpy(new_user.username, username, MAX_USERNAME - 1);
    new_user.username[MAX_USERNAME - 1] = '\0';
    strncpy(new_user.password, password, MAX_PASSWORD - 1);
    new_user.password[MAX_PASSWORD - 1] = '\0';
    strncpy(new_user.name, name, MAX_NAME - 1);
    new_user.name[MAX_NAME - 1] = '\0';
    strncpy(new_user.role, is_admin ? "Admin" : "Student", MAX_ROLE - 1);
    new_user.role[MAX_ROLE - 1] = '\0';
    new_user.failed_attempts = 0;
    new_user.is_admin = is_admin;

    fwrite(&new_user, sizeof(User), 1, fp);
    fclose(fp);
    printf("User created successfully!\n");
    return 1;
}
//=================================================================================================

void backup_database() {
    char backup_path[100];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    if (tm == NULL) {
        printf("Error getting local time for backup\n");
        return;
    }

    sprintf(backup_path, "%susers_backup_%04d%02d%02d.dat",
            BACKUP_DIR,
            tm->tm_year + 1900,
            tm->tm_mon + 1,
            tm->tm_mday);

    FILE *src = fopen(USER_FILE, "rb");
    if (!src) {
        printf("Error opening source file for backup\n");
        return;
    }

    FILE *dest = fopen(backup_path, "wb");
    if (!dest) {
        printf("Error creating backup file\n");
        fclose(src);
        return;
    }

    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dest);
    }

    fclose(src);
    fclose(dest);
    printf("Database backup created: %s\n", backup_path);
}
//=================================================================================================

int restore_database(const char *backup_file) {
    FILE *src = fopen(backup_file, "rb");
    if (!src) return 0;

    FILE *dest = fopen(USER_FILE, "wb");
    if (!dest) {
        fclose(src);
        return 0;
    }

    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytes, dest);
    }

    fclose(src);
    fclose(dest);
    printf("Database restored from: %s\n", backup_file);
    return 1;
}
