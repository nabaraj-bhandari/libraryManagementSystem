#include <stdio.h>
#include <stdlib.h>  // Include this header for atoi
#include <string.h>
#include "headers.h"

#define TRANSACTIONS_FILE "transactions.dat"
#define FINES_FILE "fines.dat"
#define MAX_BOOKS 100

// Function to generate statistics on popular books
void popular_books() {
    FILE *fp = fopen(TRANSACTIONS_FILE, "rb");
    if (!fp) {
        printf("Error opening transactions file.\n");
        return;
    }

    // Count the number of times each book has been borrowed
    int book_count[MAX_BOOKS] = {0};
    Transaction trans;
    while (fread(&trans, sizeof(Transaction), 1, fp)) {
        book_count[atoi(trans.book_id)]++;
    }
    fclose(fp);

    // Find the most popular book
    int max_count = 0;
    int popular_book_id = -1;
    for (int i = 0; i < MAX_BOOKS; i++) {
        if (book_count[i] > max_count) {
            max_count = book_count[i];
            popular_book_id = i;
        }
    }

    if (popular_book_id != -1) {
        printf("Most popular book ID: %d (borrowed %d times)\n", popular_book_id, max_count);
    } else {
        printf("No transactions found.\n");
    }
}

// Function to generate statistics on revenue from fines
void fine_revenue() {
    FILE *fp = fopen(FINES_FILE, "rb");
    if (!fp) {
        printf("Error opening fines file.\n");
        return;
    }

    double total_revenue = 0.0;
    Fine fine;
    while (fread(&fine, sizeof(Fine), 1, fp)) {
        if (fine.is_paid) {
            total_revenue += fine.amount;
        }
    }
    fclose(fp);

    printf("Total revenue from fines: $%.2f\n", total_revenue);
}
