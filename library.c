#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE_LEN 100
#define MAX_AUTHOR_LEN 100
#define MAX_DATE_LEN 15

// Structure for a book
typedef struct Book {
    int id;
    char title[MAX_TITLE_LEN];
    char author[MAX_AUTHOR_LEN];
    char publicationDate[MAX_DATE_LEN];
    struct Book* left;  // Left child in the binary tree
    struct Book* right; // Right child in the binary tree
} Book;

// Structure for a node in the linked list of borrowed books
typedef struct BookNode {
    Book* book;
    struct BookNode* next;
} BookNode;

// Structure for a user
typedef struct User {
    int id;
    BookNode* borrowedBooks; // Linked list of borrowed books
} User;

// Function to create a new book
Book* createBook(int id, const char* title, const char* author, const char* publicationDate) {
    Book* newBook = (Book*)malloc(sizeof(Book));
    newBook->id = id;
    strcpy(newBook->title, title);
    strcpy(newBook->author, author);
    strcpy(newBook->publicationDate, publicationDate);
    newBook->left = NULL;  // Initialize left child to NULL
    newBook->right = NULL; // Initialize right child to NULL
    return newBook;
}

// Function to insert a book into the binary search tree
Book* insertBook(Book* root, Book* newBook) {
    if (root == NULL) {
        return newBook;
    }
    if (newBook->id < root->id) {
        root->left = insertBook(root->left, newBook);
    } else {
        root->right = insertBook(root->right, newBook);
    }
    return root;
}

// Check if a book is borrowed by the user
int isBookBorrowed(BookNode* head, int bookId) {
    BookNode* current = head;
    while (current != NULL) {
        if (current->book->id == bookId) {
            return 1; // Book is borrowed
        }
        current = current->next;
    }
    return 0; // Book is not borrowed
}

// Function to print available books
void printAvailableBooks(Book* root, BookNode* borrowedBooks) {
    if (root != NULL) {
        printAvailableBooks(root->left, borrowedBooks);
        // Only print the book if it hasn't been borrowed
        if (!isBookBorrowed(borrowedBooks, root->id)) {
            printf("ID: %d, Title: %s, Author: %s, Publication Date: %s\n",
                   root->id, root->title, root->author, root->publicationDate);
        }
        printAvailableBooks(root->right, borrowedBooks);
    }
}

// Function to print borrowed books
void printBorrowedBooks(User* user) {
    BookNode* current = user->borrowedBooks;
    if (current == NULL) {
        printf("No borrowed books.\n");
        return;
    }
    while (current != NULL) {
        printf("Borrowed Book - ID: %d, Title: %s, Author: %s\n",
               current->book->id, current->book->title, current->book->author);
        current = current->next;
    }
}

// Function to lend a book to a user
void lendBook(User* user, Book* book) {
    if (book == NULL) {
        printf("Book not found!\n");
        return;
    }

    // Create a new node for borrowed book
    BookNode* newNode = (BookNode*)malloc(sizeof(BookNode));
    newNode->book = book;
    newNode->next = user->borrowedBooks;
    user->borrowedBooks = newNode;

    printf("Book borrowed successfully: %s\n", book->title);
}

// Function to search for a book by ID
Book* searchBook(Book* root, int bookId) {
    if (root == NULL || root->id == bookId) {
        return root;
    }
    if (bookId < root->id) {
        return searchBook(root->left, bookId);
    }
    return searchBook(root->right, bookId);
}

// Function to print the menu and handle user interaction
void printMenu(Book* bookRoot, User* user) {
    int choice;
    do {
        printf("\nOptions:\n");
        printf("1. View Available Books\n");
        printf("2. Borrow a Book\n");
        printf("3. View Borrowed Books\n");
        printf("4. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nAvailable Books:\n");
                printAvailableBooks(bookRoot, user->borrowedBooks);
                break;
            case 2: {
                int bookId;
                printf("Enter Book ID to borrow: ");
                scanf("%d", &bookId);
                Book* bookToBorrow = searchBook(bookRoot, bookId);
                lendBook(user, bookToBorrow);
                break;
            }
            case 3:
                printf("\nBorrowed Books:\n");
                printBorrowedBooks(user);
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid option! Please try again.\n");
        }
    } while (choice != 4);
}

// Function to free memory of the binary tree
void freeBookTree(Book* root) {
    if (root != NULL) {
        freeBookTree(root->left);
        freeBookTree(root->right);
        free(root);
    }
}

// Function to free borrowed books linked list
void freeBorrowedBooks(BookNode* head) {
    while (head != NULL) {
        BookNode* temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    Book* bookRoot = NULL; // Initialize bookRoot
    User user = {0}; // Initialize user

    // Sample books (BST)
    bookRoot = insertBook(bookRoot, createBook(1, "The Great Gatsby", "F. Scott Fitzgerald", "1925"));
    bookRoot = insertBook(bookRoot, createBook(2, "1984", "George Orwell", "1949"));
    bookRoot = insertBook(bookRoot, createBook(3, "To Kill a Mockingbird", "Harper Lee", "1960"));

    // Call the helper function for user interaction
    printMenu(bookRoot, &user);

    // Free allocated memory if necessary
    freeBookTree(bookRoot);
    freeBorrowedBooks(user.borrowedBooks);
    return 0;
}
