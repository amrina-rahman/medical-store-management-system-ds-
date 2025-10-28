#include<stdio.h>
#include<stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    float price;
    int quantity;
    char category[30];
    char expiry[15];
} Medicine;

typedef struct sale {
    int id;
    char name[50];
    int quantity;
    char category[30];
    float total;
    struct sale *next;
} sale;

typedef struct {
    int id;
    int quantity;
} UndoEntry;

Medicine medicines[100];
int medCount = 0;
sale *salesHead = NULL;
UndoEntry undoStack[100];
int undoTop = -1;

int searchID(int id) {
    for (int i = 0; i < medCount; i++) {
        if (medicines[i].id == id)
            return i;
    }
    return -1;
}

int searchName(char *name) {
    for (int i = 0; i < medCount; i++) {
        if (strcasecmp(medicines[i].name, name) == 0)
            return i;
    }
    return -1;
}

void pushUndo(int id, int qty) {
    undoStack[++undoTop].id = id;
    undoStack[undoTop].quantity = qty;
}

UndoEntry popUndo() {
    if (undoTop == -1) {
        UndoEntry e = {-1, 0};
        return e;
    }
    return undoStack[undoTop--];
}
//Data Structure: Linked list
void addHistory(int id, char *name, int qty, float total, char cata[30]) {
    sale *s = malloc(sizeof(sale));
    s->id = id;
    strcpy(s->name, name);
    strcpy(s->category, cata);
    s->quantity = qty;
    s->total = total;
    s->next = salesHead;
    salesHead = s;
}

void viewHistory() {
    sale *temp = salesHead;
    printf("\n--- Sales History ---\n");
    while (temp != NULL) {
        printf("ID: %d | %s | Qty: %d | Total: %.2f\n",
               temp->id, temp->name, temp->quantity, temp->total);
        temp = temp->next;
    }
}

void addMedicine() {
    Medicine m;
    printf("Enter Medicine ID: ");
    scanf("%d", &m.id);
    printf("Enter Name: ");
    scanf(" %[^\n]", m.name);
    printf("Enter Price: ");
    scanf("%f", &m.price);
    printf("Enter Quantity: ");
    scanf("%d", &m.quantity);
    printf("Enter Catagory:");
    scanf(" %[^\n]", m.category);
     printf("Enter Expiry: ");
    scanf(" %[^\n]", m.expiry);

    medicines[medCount++] = m;
    printf("Medicine added successfully!\n");
}
//Data Structure: Linear Search
void searchMedicine() {
    int choice;
    printf("Search by:\n 1.ID \n 2.Name: \n");
    scanf("%d", &choice);

    int result = -1;
    if (choice == 1) {
        int id;
        printf("Enter ID: ");
        scanf("%d", &id);
        result = searchID(id);
    } else if (choice == 2) {
        char name[50];
        printf("Enter Name: ");
        scanf(" %[^\n]", name);
        result = searchName(name);
    }

    if (result == -1) {
        printf("Medicine not found.\n");
    } else {
        printf("ID: %d | Name: %s | Catagory:%s | Price: %.2f | Qty: %d | Expiry: %s\n",
               medicines[result].id, medicines[result].name,medicines[result].category,
               medicines[result].price, medicines[result].quantity, medicines[result].expiry);
    }
}
void sellMedicine() {
    int qty;
    char name[50];
    printf("Enter Medicine name to sell: ");

    scanf(" %[^\n]", name);

    int sell = searchName(name);
    if (sell == -1) {
        printf("Medicine not found.\n");
        return;
    }
    printf("Enter quantity: ");
    scanf("%d", &qty);

    if (qty > medicines[sell].quantity) {
        printf("Not enough stock.\n");
        return;
    }
  medicines[sell].quantity -= qty;
    float total = qty * medicines[sell].price;
    addHistory(medicines[sell].id, medicines[sell].name, qty,total , medicines[sell].category);
    pushUndo(medicines[sell].id, qty);
    printf("Sold successfully! Total: %.2f\n", total);
}

//Data Structure: Stack
void undoSale() {
    if (undoTop == -1) {
        printf("Nothing to undo.\n");
        return;
    }

    UndoEntry e = popUndo();
    int idx = searchID(e.id);
    if (idx != -1) {
        medicines[idx].quantity += e.quantity;
        printf("Last sale undone.\n");
    }
}
void lowStockAlert() {
    printf("\n--- Low Stock Medicines (<=5) ---\n");
    for (int i = 0; i < medCount; i++) {
        if (medicines[i].quantity <= 5) {
            printf("%s | Qty: %d\n", medicines[i].name, medicines[i].quantity);
        }
    }
}
int main() {
    int choice;
    do {
        printf("\n--- Medical Store Management ---\n");
        printf("1. Add Medicine\n");
        printf("2. Search Medicine\n");
        printf("3. Sell Medicine\n");
        printf("4. Undo Last Sale\n");
        printf("5. View Sales History\n");
        printf("6. Low Stock Alert\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addMedicine();
             break;
            case 2: searchMedicine();
             break;
            case 3: sellMedicine();
             break;
            case 4: undoSale();
             break;
            case 5: viewHistory(); 
            break;
            case 6: lowStockAlert();
             break;
            case 0: printf("Exiting...\n"); 
            break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);
    return 0;
}

