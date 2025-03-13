// main.c
#include <stdio.h>    
#include <string.h>  
#include <stdlib.h> 
#include "headers.h"

#define USERS_FILE "users.dat"

// Define the MenuItem structure
typedef struct {
    char title[50];
    void (*function)();
} MenuItem;

// Function prototypes
void clear_input_buffer();
int login_system();
int create_account();
void logout_system();
void display_main_menu();
void change_own_details();
void handle_borrow_book();
void handle_return_book();
void exit_program();
void wait_for_enter();
void display_menu(const char* header, MenuItem* items, int count);
void handle_admin_menu();
void handle_student_menu();
void display_available_books();
void display_borrowed_books();

User current_user = {0};

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int login_system() {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    User user;

    print_header("Library Management System");
    printf("Enter username: ");
    scanf("%19s", username);
    clear_input_buffer();

    printf("Enter password: ");
    scanf("%19s", password);
    clear_input_buffer();

    if (authenticate_user(username, password, &user)) {
        current_user = user;
        printf("Login successful! Welcome, %s\n", user.name);
        return 1;
    } else {
        printf("Login failed. Please try again.\n");
        return 0;
    }
}

int create_account() {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char name[MAX_NAME];

    print_header("Create New Account");
    printf("Enter username: ");
    scanf("%19s", username);
    clear_input_buffer();

    printf("Enter password: ");
    scanf("%19s", password);
    clear_input_buffer();

    printf("Enter your full name: ");
    fgets(name, MAX_NAME, stdin);
    name[strcspn(name, "\n")] = 0;

    return create_user(username, password, name, 0);
}

void logout_system() {
    printf("Logging out %s...\n", current_user.name);
    memset(&current_user, 0, sizeof(User));
    printf("Logged out successfully.\n");
    // Return to main menu
    while (1) {
        display_main_menu();
        int choice = get_menu_choice(1, 3);

        switch (choice) {
            case 1: // Login
                if (login_system()) {
                    if (strcmp(current_user.role, "Admin") == 0) {
                        handle_admin_menu();
                    } else {
                        handle_student_menu();
                    }
                }
                break;
            case 2: // Create Account
                create_account();
                break;
            case 3: // Exit
                printf("Exiting...\n");
                exit_program();
                return;
            default:
                printf("Invalid choice\n");
        }
    }
}

void display_main_menu() {
    print_header("Library Management System");
    printf("| Welcome to the Library System!        |\n");
    print_divider();
    printf("| 1. Login                             |\n");
    printf("| 2. Create Account                    |\n");
    printf("| 3. Exit                              |\n");
    print_divider();
}

void display_menu(const char* header, MenuItem* items, int count) {
    clear_screen();
    print_header(header);
    print_divider();
    
    for (int i = 0; i < count; i++) {
        printf("%d. %s\n", i + 1, items[i].title);
    }
    print_divider();
    
    int choice = get_menu_choice(1, count) - 1;
    if (choice >= 0 && choice < count) {
        items[choice].function();
    }
}

void handle_admin_menu() {
    while (1) {
        MenuItem admin_items[] = {
            {"Manage Books", manage_books},
            {"Manage Users", manage_users},
            {"Generate Reports", generate_reports},
            {"View Reports", view_reports},
            {"Logout", logout_system}
        };
        
        display_menu("Admin Menu", admin_items, 5);
    }
}

void handle_student_menu() {
    while (1) {
        MenuItem student_items[] = {
            {"Borrow Book", handle_borrow_book},
            {"Return Book", handle_return_book},
            {"Change Details", change_own_details},
            {"Logout", logout_system}
        };
        
        display_menu("Student Menu", student_items, 4);
    }
}

void handle_borrow_book() {
    display_available_books();
    char book_id[MAX_BOOK_ID];
    printf("Enter Book ID or 'q' to exit: ");
    scanf("%9s", book_id);
    clear_input_buffer();
    if (strcmp(book_id, "q") == 0) return;
    borrow_book(current_user.username, book_id);
}

void handle_return_book() {
    display_borrowed_books();
    char book_id[MAX_BOOK_ID];
    printf("Enter Book ID or 'q' to exit: ");
    scanf("%9s", book_id);
    clear_input_buffer();
    if (strcmp(book_id, "q") == 0) return;
    return_book(current_user.username, book_id);
}

void change_own_details() {
    User user = current_user;
    printf("Enter new Password: ");
    fgets(user.password, MAX_PASSWORD, stdin);
    user.password[strcspn(user.password, "\n")] = 0;
    printf("Enter new Full Name: ");
    fgets(user.name, MAX_NAME, stdin);
    user.name[strcspn(user.name, "\n")] = 0;

    FILE *fp = fopen(USERS_FILE, "rb+");
    if (!fp) {
        printf("Error opening users file.\n");
        return;
    }

    User temp;
    long pos;
    while (fread(&temp, sizeof(User), 1, fp)) {
        if (strcmp(temp.username, user.username) == 0) {
            pos = ftell(fp) - sizeof(User);
            fseek(fp, pos, SEEK_SET);
            fwrite(&user, sizeof(User), 1, fp);
            current_user = user;
            printf("Details updated successfully!\n");
            break;
        }
    }

    fclose(fp);
}

void exit_program() {
    printf("Thank you for using the Library Management System. Goodbye!\n");
    exit(0);
}

int main() {
    database_init();

    while (1) {
        display_main_menu();
        int choice = get_menu_choice(1, 3);

        switch (choice) {
            case 1: // Login
                if (login_system()) {
                    if (strcmp(current_user.role, "Admin") == 0) {
                        handle_admin_menu();
                    } else {
                        handle_student_menu();
                    }
                }
                break;
            case 2: // Create Account
                create_account();
                break;
            case 3: // Exit
                printf("Exiting...\n");
                exit_program();
                return 0;
            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}
