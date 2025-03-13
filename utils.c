// utils.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep() on Unix-like systems
#include "headers.h"

void print_header(const char *title) {
    printf("\n================ %s ================\n", title);
}

void print_divider() {
    printf("============================================\n");
}

int get_menu_choice(int min, int max) {
    int choice;
    char input[10];
    printf("Enter your choice (%d-%d): ", min, max);
    fgets(input, sizeof(input), stdin);
    choice = atoi(input);
    if (choice < min || choice > max) {
        printf("Invalid choice. Please try again.\n");
        return get_menu_choice(min, max);
    }
    return choice;
}

void clear_screen() {
    printf("Press Enter to continue...");
    getchar();
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void display_admin_menu() {
    print_header("Admin Menu");
    printf("| 1. Manage Books                     |\n");
    printf("| 2. Manage Users                     |\n");
    printf("| 3. Logout                           |\n");
    print_divider();
}

void display_student_menu() {
    print_header("Student Menu");
    printf("| 1. Borrow Book                      |\n");
    printf("| 2. Return Book                      |\n");
    printf("| 3. Logout                           |\n");
    print_divider();
}
