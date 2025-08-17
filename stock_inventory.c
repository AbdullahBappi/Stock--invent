
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXPRODUCTS 50
#define NAMELEN 30
#define STACKSIZE 50
#define QUEUESIZE 50


typedef struct {
    int id;
    char name[NAMELEN];
    int stock;
    float price;
} Product;

Product products[MAXPRODUCTS];
int productCount = 0;

typedef struct OrderNode {
    char customerName[NAMELEN];
    int productId;
    int quantity;
    struct OrderNode *next;
} OrderNode;

OrderNode *orderHead = NULL;


typedef struct {
    int productId;
    int quantity;
} Sale;

Sale salesStack[STACKSIZE];
int top = -1;


typedef struct {
    int productId;
    int quantity;
} Restock;

Restock restockQueue[QUEUESIZE];
int front = 0, rear = -1, restockCount = 0;


void addProduct() {
    if (productCount >= MAXPRODUCTS) {
        printf("Product list full!\n");
        return;
    }
    Product p;
    printf("Enter Product ID: ");
    scanf("%d", &p.id);
    printf("Enter Product Name: ");
    scanf(" %[^\n]", p.name);
    printf("Enter Stock Quantity: ");
    scanf("%d", &p.stock);
    printf("Enter Price: ");
    scanf("%f", &p.price);
    products[productCount++] = p;
    printf("Product added successfully!\n");
}

void displayProducts() {
    if (productCount == 0) {
        printf("No products available.\n");
        return;
    }
    printf("\n--- Product List ---\n");
    for (int i = 0; i < productCount; i++) {
        printf("ID: %d | Name: %s | Stock: %d | Price: %.2f\n",
               products[i].id, products[i].name, products[i].stock, products[i].price);
    }
}

void placeOrder() {
    OrderNode *newOrder = (OrderNode *)malloc(sizeof(OrderNode));
    printf("Enter Customer Name: ");
    scanf(" %[^\n]", newOrder->customerName);
    printf("Enter Product ID: ");
    scanf("%d", &newOrder->productId);
    printf("Enter Quantity: ");
    scanf("%d", &newOrder->quantity);
    newOrder->next = NULL;

    if (orderHead == NULL) {
        orderHead = newOrder;
    } else {
        OrderNode *temp = orderHead;
        while (temp->next) temp = temp->next;
        temp->next = newOrder;
    }
    printf("Order placed successfully!\n");
}

void showOrders() {
    if (orderHead == NULL) {
        printf("No orders.\n");
        return;
    }
    printf("\n--- Orders ---\n");
    OrderNode *temp = orderHead;
    while (temp) {
        printf("Customer: %s | Product ID: %d | Qty: %d\n",
               temp->customerName, temp->productId, temp->quantity);
        temp = temp->next;
    }
}

void sellProduct() {
    int id, qty, found = -1;
    printf("Enter Product ID: ");
    scanf("%d", &id);
    printf("Enter Quantity: ");
    scanf("%d", &qty);

    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Product not found!\n");
        return;
    }

    if (products[found].stock < qty) {
        printf("Not enough stock!\n");
        return;
    }

    products[found].stock -= qty;
    if (top < STACKSIZE - 1) {
        top++;
        salesStack[top].productId = id;
        salesStack[top].quantity = qty;
    }
    printf("Sale recorded successfully!\n");
}

void undoSale() {
    if (top == -1) {
        printf("No sales to undo.\n");
        return;
    }
    int id = salesStack[top].productId;
    int qty = salesStack[top].quantity;
    top--;

    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id) {
            products[i].stock += qty;
            printf("Sale undone: Product ID %d, Qty %d restored.\n", id, qty);
            return;
        }
    }
}


void addRestockRequest() {
    if (restockCount >= QUEUESIZE) {
        printf("Restock queue full!\n");
        return;
    }
    rear = (rear + 1) % QUEUESIZE;
    printf("Enter Product ID: ");
    scanf("%d", &restockQueue[rear].productId);
    printf("Enter Quantity: ");
    scanf("%d", &restockQueue[rear].quantity);
    restockCount++;
    printf("Restock request added!\n");
}

void processRestock() {
    if (restockCount == 0) {
        printf("No restock requests.\n");
        return;
    }
    Restock r = restockQueue[front];
    front = (front + 1) % QUEUESIZE;
    restockCount--;

    for (int i = 0; i < productCount; i++) {
        if (products[i].id == r.productId) {
            products[i].stock += r.quantity;
            printf("Restocked Product ID %d by %d units.\n", r.productId, r.quantity);
            return;
        }
    }
    printf("Restock failed: Product not found.\n");
}


int main() {
    int choice;
    while (1) {
        printf("\n--- Stock & Inventory Management ---\n");
        printf("1. Add Product\n");
        printf("2. Display Products\n");
        printf("3. Sell Product\n");
        printf("4. Undo Last Sale\n");
        printf("5. Place Order\n");
        printf("6. Show Orders\n");
        printf("7. Add Restock Request\n");
        printf("8. Process Restock\n");
        printf("9. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addProduct();
             break;
            case 2: displayProducts();
             break;
            case 3: sellProduct();
            break;
            case 4: undoSale();
            break;
            case 5: placeOrder();
            break;
            case 6: showOrders();
            break;
            case 7: addRestockRequest();
            break;
            case 8: processRestock();
            break;
            case 9: printf("Exiting...\n");
            return 0;
            default: printf("Invalid choice!\n");
        }
    }
}
