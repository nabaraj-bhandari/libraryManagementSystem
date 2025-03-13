#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // Include this header for atoi
#include "headers.h"

#define BOOKS_FILE "books.dat"
#define REPORTS_FILE "reports.csv"
#define USERS_FILE "users.dat"

// Function prototypes
void add_book();
void remove_book();
void update_book();
void display_all_books();
void generate_reports();
void view_reports();
void add_user();
void remove_user();
void update_user();
void display_all_users();
void manage_books();
void manage_users();

int get_next_book_id(const char* book_code) {
    FILE *fp = fopen(BOOKS_FILE, "rb");
    if (!fp) {
        return 1;  // Start with 001 if file doesn't exist
    }

    Book book;
    int max_id = 0;
    while (fread(&book, sizeof(Book), 1, fp)) {
        if (strncmp(book.book_id, book_code, 3) == 0) {
            int id = atoi(book.book_id + 3);
            if (id > max_id) {
                max_id = id;
            }
        }
    }

    fclose(fp);
    return max_id + 1;
}

// Function to add a new book
void add_book() {
    Book book;
    char book_code[4];
    printf("Enter unique book code (e.g., PHY) or 'q' to abort: ");
    scanf("%3s", book_code);
    clear_input_buffer();
    if (strcmp(book_code, "q") == 0) return;

    int book_id_num = get_next_book_id(book_code);
    snprintf(book.book_id, MAX_BOOK_ID, "%s%03d", book_code, book_id_num);

    printf("Enter Title: ");
    fgets(book.title, MAX_TITLE, stdin);
    book.title[strcspn(book.title, "\n")] = 0;
    printf("Enter Author: ");
    fgets(book.author, MAX_AUTHOR, stdin);
    book.author[strcspn(book.author, "\n")] = 0;
    book.is_available = 1;

    FILE *fp = fopen(BOOKS_FILE, "ab");
    if (!fp) {
        printf("Error opening books file.\n");
        return;
    }
    fwrite(&book, sizeof(Book), 1, fp);
    fclose(fp);
    printf("Book added successfully!\n");
}

// Function to remove a book
void remove_book() {
    display_all_books();
    char book_id[MAX_BOOK_ID];
    printf("Enter Book ID to remove or 'q' to abort: ");
    scanf("%9s", book_id);
    clear_input_buffer();
    if (strcmp(book_id, "q") == 0) return;

    FILE *fp = fopen(BOOKS_FILE, "rb");
    if (!fp) {
        printf("Error opening books file.\n");
        return;
    }

    FILE *temp_fp = fopen("temp.dat", "wb");
    if (!temp_fp) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    Book book;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, fp)) {
        if (strcmp(book.book_id, book_id) != 0) {
            fwrite(&book, sizeof(Book), 1, temp_fp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp_fp);

    if (found) {
        remove(BOOKS_FILE);
        rename("temp.dat", BOOKS_FILE);
        printf("Book removed successfully!\n");
    } else {
        remove("temp.dat");
        printf("Book ID not found.\n");
    }
}

// Function to update a book
void update_book() {
    display_all_books();

    char book_id[MAX_BOOK_ID];
    printf("Enter Book ID to update or 'q' to abort: ");
    scanf("%9s", book_id);
    clear_input_buffer();
    if (strcmp(book_id, "q") == 0) return;

    FILE *fp = fopen(BOOKS_FILE, "rb+");
    if (!fp) {
        printf("Error opening books file.\n");
        return;
    }

    Book book;
    long pos;
    int found = 0;
    while (fread(&book, sizeof(Book), 1, fp)) {
        if (strcmp(book.book_id, book_id) == 0) {
            found = 1;
            pos = ftell(fp) - sizeof(Book);
            printf("Enter new Title: ");
            fgets(book.title, MAX_TITLE, stdin);
            book.title[strcspn(book.title, "\n")] = 0;
            printf("Enter new Author: ");
            fgets(book.author, MAX_AUTHOR, stdin);
            book.author[strcspn(book.author, "\n")] = 0;
            fseek(fp, pos, SEEK_SET);
            fwrite(&book, sizeof(Book), 1, fp);
            printf("Book updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Book ID not found.\n");
    }

    fclose(fp);
}

// Function to display all books
void display_all_books() {
    FILE *fp = fopen(BOOKS_FILE, "rb");
    if (!fp) {
        printf("Error opening books file.\n");
        return;
    }

    Book book;
    printf("\nAll Books:\n");
    printf("ID\t\tTitle\t\t\t\tAuthor\t\t\t\tAvailability\n");
    printf("----------------------------------------------------------------------------------------------------------\n");
    while (fread(&book, sizeof(Book), 1, fp)) {
        printf("%-15s\t%-30s\t%-30s\t%s\n", book.book_id, book.title, book.author, book.is_available ? "Available" : "Borrowed");
    }

    fclose(fp);
}

// Function to generate reports
void generate_reports() {
    FILE *fp = fopen(REPORTS_FILE, "w");
    if (!fp) {
        printf("Error creating reports file.\n");
        return;
    }

    fprintf(fp, "Book ID,Title,Author,Availability\n");

    FILE *books_fp = fopen(BOOKS_FILE, "rb");
    if (!books_fp) {
        printf("Error opening books file.\n");
        fclose(fp);
        return;
    }

    Book book;
    while (fread(&book, sizeof(Book), 1, books_fp)) {
        fprintf(fp, "%s,%s,%s,%s\n", book.book_id, book.title, book.author, book.is_available ? "Available" : "Borrowed");
    }

    fclose(books_fp);
    fclose(fp);
    printf("Reports generated successfully!\n");
}

// Function to view reports
void view_reports() {
    FILE *fp = fopen(REPORTS_FILE, "r");
    if (!fp) {
        printf("Error opening reports file.\n");
        return;
    }

    char line[256];
    printf("\nGenerated Reports:\n");
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ",");
        printf("%-15s\t", token);
        token = strtok(NULL, ",");
        printf("%-30s\t", token);
        token = strtok(NULL, ",");
        printf("%-30s\t", token);
        token = strtok(NULL, ",");
        printf("%s", token);
    }

    fclose(fp);
}

// Function to add a new user
void add_user() {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char name[MAX_NAME];
    int is_admin;

    printf("Enter username or 'q' to abort: ");
    scanf("%19s", username);
    clear_input_buffer();
    if (strcmp(username, "q") == 0) return;

    printf("Enter password: ");
    scanf("%19s", password);
    clear_input_buffer();

    printf("Enter full name: ");
    fgets(name, MAX_NAME, stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Is admin (1 for yes, 0 for no): ");
    scanf("%d", &is_admin);
    clear_input_buffer();

    if (create_user(username, password, name, is_admin)) {
        printf("User added successfully!\n");
    } else {
        printf("Failed to add user.\n");
    }
}

// Function to remove a user
void remove_user() {
    display_all_users();
    char username[MAX_USERNAME];
    printf("Enter Username to remove or 'q' to abort: ");
    scanf("%19s", username);
    clear_input_buffer();
    if (strcmp(username, "q") == 0) return;

    FILE *fp = fopen(USERS_FILE, "rb");
    if (!fp) {
        printf("Error opening users file.\n");
        return;
    }

    FILE *temp_fp = fopen("temp.dat", "wb");
    if (!temp_fp) {
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    User user;
    int found = 0;
    while (fread(&user, sizeof(User), 1, fp)) {
        if (strcmp(user.username, username) != 0) {
            fwrite(&user, sizeof(User), 1, temp_fp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp_fp);

    if (found) {
        remove(USERS_FILE);
        rename("temp.dat", USERS_FILE);
        printf("User removed successfully!\n");
    } else {
        remove("temp.dat");
        printf("Username not found.\n");
    }
}

// Function to update a user
void update_user() {
    display_all_users();

    char username[MAX_USERNAME];
    printf("Enter Username to update or 'q' to abort: ");
    scanf("%19s", username);
    clear_input_buffer();
    if (strcmp(username, "q") == 0) return;

    FILE *fp = fopen(USERS_FILE, "rb+");
    if (!fp) {
        printf("Error opening users file.\n");
        return;
    }

    User user;
    long pos;
    int found = 0;
    while (fread(&user, sizeof(User), 1, fp)) {
        if (strcmp(user.username, username) == 0) {
            found = 1;
            pos = ftell(fp) - sizeof(User);
            printf("Enter new Password: ");
            fgets(user.password, MAX_PASSWORD, stdin);
            user.password[strcspn(user.password, "\n")] = 0;
            printf("Enter new Full Name: ");
            fgets(user.name, MAX_NAME, stdin);
            user.name[strcspn(user.name, "\n")] = 0;
            fseek(fp, pos, SEEK_SET);
            fwrite(&user, sizeof(User), 1, fp);
            printf("User updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Username not found.\n");
    }

    fclose(fp);
}

// Function to display all users
void display_all_users() {
    FILE *fp = fopen(USERS_FILE, "rb");
    if (!fp) {
        printf("Error opening users file.\n");
        return;
    }

    User user;
    printf("\nAll Users:\n");
    printf("Username\t\tName\t\t\tRole\n");
    printf("------------------------------------------------------------\n");
    while (fread(&user, sizeof(User), 1, fp)) {
        printf("%-20s\t%-30s\t%s\n", user.username, user.name, user.role);
    }

    fclose(fp);
}

// Function to manage books
void manage_books() {
    while (1) {
        clear_screen();
        print_header("Manage Books");
        printf("| 1. Add Book                         |\n");
        printf("| 2. Remove Book                      |\n");
        printf("| 3. Update Book                      |\n");
        printf("| 4. Display All Books                |\n");
        printf("| 5. Back to Admin Menu               |\n");
        print_divider();
        int choice = get_menu_choice(1, 5);

        switch (choice) {
            case 1:
                clear_screen();
                add_book();
                break;
            case 2:
                clear_screen();
                remove_book();
                break;
            case 3:
                clear_screen();
                update_book();
                break;
            case 4:
                clear_screen();
                display_all_books();
                break;
            case 5:
                return;  // Correctly return to the admin menu
            default:
                printf("Invalid choice\n");
        }
    }
}

// Function to manage users
void manage_users() {
    while (1) {
        clear_screen();
        print_header("Manage Users");
        printf("| 1. Add User                         |\n");
        printf("| 2. Remove User                      |\n");
        printf("| 3. Update User                      |\n");
        printf("| 4. Display All Users                |\n");
        printf("| 5. Back to Admin Menu               |\n");
        print_divider();
        int choice = get_menu_choice(1, 5);

        switch (choice) {
            case 1:
                clear_screen();
                add_user();
                break;
            case 2:
                clear_screen();
                remove_user();
                break;
            case 3:
                clear_screen();
                update_user();
                break;
            case 4:
                clear_screen();
                display_all_users();
                break;
            case 5:
                return;  // Correctly return to the admin menu
            default:
                printf("Invalid choice\n");
        }
    }
}
