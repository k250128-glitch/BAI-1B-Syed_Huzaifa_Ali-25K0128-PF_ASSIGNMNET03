#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    int popularity;
    int lastAccess;
} Book;

void printShelf(Book *shelf, int bookCount) {

    printf("\n================== SHELF STATUS ==================\n\n");

    if(bookCount == 0) {
        printf("Shelf is empty.\n");
        printf("=================================================\n\n");
        return;
    }

    printf(" %-8s %-15s %-12s\n", "ID", "POPULARITY", "LAST USED");
    printf(" -------------------------------------------------\n");

    Book *bookPtr;
    for(bookPtr = shelf; bookPtr < shelf + bookCount; bookPtr++) {
        printf(" %-8d %-15d %-12d\n",
               bookPtr->id,
               bookPtr->popularity,
               bookPtr->lastAccess);
    }

    printf(" -------------------------------------------------\n\n");
}
// MADE BY HUZIAFA
int main() {
    int shelfSize, operations, idx, k;
    prinf("STAAAART GETTTING READDDY CCOODE BY HUZAIFA IS HERE\n\n");
    printf("Enter shelf capacity: ");
    scanf("%d", &shelfSize);

    printf("Enter number of operations: ");
    scanf("%d", &operations);

    Book shelf[shelfSize];
    Book *bookPtr;

    int bookCount = 0;
    int globalTime = 0;

    for(idx = 0; idx < operations; idx++) {

        char command[10];
        printf("\nOperation %d (ADD x y / ACCESS x): ", idx + 1);
        scanf("%s", command);

        if(strcmp(command, "ADD") == 0) {
            int bookID, bookPopularity;

            printf("Enter Book ID and Popularity: ");
            scanf("%d %d", &bookID, &bookPopularity);

            globalTime++;

            int indexFound = -1;

            for(bookPtr = shelf; bookPtr < shelf + bookCount; bookPtr++) {
                if(bookPtr->id == bookID) {
                    indexFound = (int)(bookPtr - shelf);
                    break;
                }
            }

            if(indexFound != -1) {
                (shelf + indexFound)->popularity = bookPopularity;
                (shelf + indexFound)->lastAccess = globalTime;
                printf(" Updated existing book → ID: %d | Popularity: %d\n",
                       bookID, bookPopularity);
            }
            else {
                if(bookCount == shelfSize) {

                    int oldestIndex = 0;

                    for(bookPtr = shelf + 1; bookPtr < shelf + bookCount; bookPtr++) {
                        if(bookPtr->lastAccess < (shelf + oldestIndex)->lastAccess)
                            oldestIndex = (int)(bookPtr - shelf);
                    }

                    printf(" Shelf full → Removing LRU Book ID: %d\n",
                           (shelf + oldestIndex)->id);

                    for(k = oldestIndex; k < bookCount - 1; k++) {
                        shelf[k] = shelf[k + 1];
                    }

                    bookCount--;
                }

                shelf[bookCount].id = bookID;
                shelf[bookCount].popularity = bookPopularity;
                shelf[bookCount].lastAccess = globalTime;
                bookCount++;

                printf(" Book added → ID: %d | Popularity: %d\n",
                       bookID, bookPopularity);
            }

        }
        else if(strcmp(command, "ACCESS") == 0) {

            int bookID;
            printf("Enter Book ID to access: ");
            scanf("%d", &bookID);

            globalTime++;

            int indexFound = -1;

            for(bookPtr = shelf; bookPtr < shelf + bookCount; bookPtr++) {
                if(bookPtr->id == bookID) {
                    indexFound = (int)(bookPtr - shelf);
                    break;
                }
            }

            if(indexFound == -1) {
                printf(" Book not found → -1\n");
            }
            else {
                (shelf + indexFound)->lastAccess = globalTime;
                printf(" Accessed Book → ID: %d | Popularity: %d\n",
                       bookID, (shelf + indexFound)->popularity);
            }
        }

        printShelf(shelf, bookCount);
    }
    printf("CIDE BY SYED HUZAIFA ALI")

    return 0;
}

