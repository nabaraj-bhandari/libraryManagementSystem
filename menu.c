#include "headers.h"

typedef struct {
    const char* title;
    void (*function)();
} MenuItem;

void clear_screen();

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
    MenuItem admin_items[] = {
        {"Manage Books", manage_books},
        {"Manage Users", manage_users},
        {"Generate Reports", generate_reports},
        {"View Reports", view_reports},
        {"Popular Books", popular_books},
        {"Fine Revenue", fine_revenue},
        {"Logout", logout_system}
    };
    
    display_menu("Admin Menu", admin_items, 7);
}

void handle_student_menu() {
    MenuItem student_items[] = {
        {"Borrow Book", handle_borrow_book},
        {"Return Book", handle_return_book},
        {"Change Details", change_own_details},
        {"Logout", logout_system}
    };
    
    display_menu("Student Menu", student_items, 4);
}
