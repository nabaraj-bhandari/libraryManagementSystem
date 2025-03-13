// headers.h
#ifndef HEADERS_H
#define HEADERS_H

#include <stdio.h>  
#include <time.h>   

#define MAX_USERNAME 20
#define MAX_PASSWORD 20
#define MAX_NAME 50
#define MAX_ROLE 10
#define MAX_TITLE 100
#define MAX_AUTHOR 50
#define MAX_BOOK_ID 10

// User structure
typedef struct {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char name[MAX_NAME];
    char role[MAX_ROLE];
    int failed_attempts;
    int is_admin;
} User;

// Book structure
typedef struct {
    char book_id[MAX_BOOK_ID];
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    int is_available;
} Book;

// Transaction structure
typedef struct {
    char book_id[MAX_BOOK_ID];
    char username[MAX_USERNAME];
    time_t issue_date;
    time_t return_date;  // 0 if not returned yet
} Transaction;

// Fine structure
typedef struct {
    char username[MAX_USERNAME];
    double amount;
    int is_paid;
} Fine;

// Function prototypes (database.c)
int authenticate_user(char* username, char* password, User* user);  // Updated prototype
int create_user(char* username, char* password, char* name, int is_admin);
void database_init();
int validate_string(const char* str, int max_len);
void backup_database();
int restore_database(const char *backup_file);

// Function prototypes (utils.c)
void print_header(const char* title);
void print_divider();
int get_menu_choice(int min, int max);
void display_admin_menu();
void display_student_menu();
void clear_screen();

// Function prototypes (student.c)
int borrow_book(const char* username, const char* book_id);
int return_book(const char* username, const char* book_id);
int reserve_book(const char* username, const char* book_id);
int check_book_availability(const char* book_id);
void display_available_books();
void display_borrowed_books();

// Function prototypes (transactions.c)
int issue_book(const char* username, const char* book_id);
int return_book_transaction(const char* username, const char* book_id);
double calculate_fine(const char* username, const char* book_id);
void update_fine(const char* username, double amount);
double get_user_fines(const char* username);

// Function prototypes (admin.c)
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

// Function prototypes (main.c)
void clear_input_buffer();
int login_system();  // Updated prototype
int create_account();
void logout_system();
void display_main_menu();
void change_own_details();
void handle_borrow_book();
void handle_return_book();
void exit_program();
void wait_for_enter();

#endif
