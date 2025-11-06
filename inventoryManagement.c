#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Product
{
    int id;
    char name[50];
    float price;
    int quantity;
} Product;

typedef enum
{
    ADD_PRODUCT = 1,
    VIEW_PRODUCTS,
    UPDATE_QUANTITY,
    SEARCH_BY_ID,
    SEARCH_BY_NAME,
    SEARCH_BY_PRICE_RANGE,
    DELETE,
    EXIT
} MenuOptions;

void removeNewLine(char *text)
{
    for (char *character = text; *character != '\0'; character++)
    {
        if (*character == '\n')
        {
            *character = '\0';
            break;
        }
    }
}

bool isSubstringExists(const char *originalString, const char *subString)
{
    bool found = false;
    if (*subString != '\0')
    {
        const char *ptrOriginal = originalString;
        while (*ptrOriginal != '\0' && !found)
        {
            const char *ptr = ptrOriginal;
            const char *ptrSubstring = subString;
            while (*ptr != '\0' && *ptrSubstring != '\0' && *ptr == *ptrSubstring)
            {
                ptr++;
                ptrSubstring++;
            }
            if (*ptrSubstring == '\0')
            {
                found = true;
            }
            ptrOriginal++;
        }
    }
    else
    {
        found = true;
    }
    return found;
}

void printMenu()
{
    printf("\n\n=====INVENTORY MENU======\n");
    printf("%d. Add New Product\n", ADD_PRODUCT);
    printf("%d. View All Products\n", VIEW_PRODUCTS);
    printf("%d. Update Quantity\n", UPDATE_QUANTITY);
    printf("%d. Search Product by ID\n", SEARCH_BY_ID);
    printf("%d. Search Product by Name\n", SEARCH_BY_NAME);
    printf("%d. Search Product by Price Range\n", SEARCH_BY_PRICE_RANGE);
    printf("%d. Delete Product\n", DELETE);
    printf("%d. Exit\n", EXIT);
}
void printDetails(Product *product)
{
    printf("\nProduct ID : %d | Name : %s | Price : %.2f | Quantity : %d\n",
           product->id,
           product->name,
           product->price,
           product->quantity);
}

void addProduct(Product** products, int *capacity, int *currentCount)
{
    if (*currentCount >= *capacity)
    {
        *capacity *= 2;
        Product *temp = (Product *)realloc(*products, (*capacity) * sizeof(Product));
        if (temp == NULL)
        {
            return;
        }
        *products = temp;
    }
    Product *newProduct = *products + *currentCount;
    printf("\nEnter details for product %d: \n", *currentCount+1);
    printf("\nProduct Id : ");
    scanf("%d", &newProduct->id);
    printf("Product Name : ");
    getchar();
    fgets(newProduct -> name, sizeof(newProduct -> name), stdin);
    removeNewLine(newProduct->name);
    printf("Product Price : ");
    scanf("%f", &newProduct->price);
    printf("Product Quantity : ");
    scanf("%d", &newProduct->quantity);
    (*currentCount)++;
    printf("\nproduct added successfully!\n");
}

void displayProducts(Product *products, const int currentCount)
{
    if (currentCount != 0)
    {
        printf("\nPRODUCT LIST\n");
        for (Product *ptr = products; ptr < products + currentCount; ptr++)
        {
            printDetails(ptr);
        }
    }
    else
    {
        printf("No Product details found");
    }
}

void updateQuantity(Product *products, const int currentCount)
{
    int productId, newQuantity;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &productId);
    printf("Enter new Quantity: ");
    scanf("%d", &newQuantity);
    for (Product *ptr = products; ptr < products + currentCount; ptr++)
    {
        if (ptr -> id == productId)
        {
            ptr -> quantity = newQuantity;
            printf("\nQuantity updated successfully!");
            return;
        }
    }
    printf("\n Product ID %d not found.\n", productId);
}

void searchProductById(Product *products, const int currentCount)
{
    int searchId;
    printf("Enter Product ID to search: ");
    scanf("%d", &searchId);
    for (Product *ptr = products; ptr < products + currentCount; ptr++)
    {
        if (ptr -> id == searchId)
        {
            printf("\nProduct found: ");
            printDetails(ptr);
            return;
        }
    }
    printf("\n Product ID %d not found.\n", searchId);
}

void searchProductByName(Product *products, const int currentCount)
{
    char searchName[50];
    bool found = false;
    printf("\nEnter product name to search: ");
    getchar();
    fgets(searchName, sizeof(searchName), stdin);
    removeNewLine(searchName);
    for (Product *ptr = products; ptr < products + currentCount; ptr++)
    {
        if (isSubstringExists(ptr->name, searchName))
        {
            if (!found)
            {
                printf("\nProducts found: \n");
            }
            printDetails(ptr);
            found = true;
        }
    }
    if (!found)
    {
        printf("\n No products found with the name matching %s.\n", searchName);
    }
}

void searchProductsByPriceRange(Product *products, const int currentCount)
{
    float minimumPrice, maximumPrice;
    bool found = false;
    printf("Enter minimum Price: ");
    scanf("%f", &minimumPrice);
    printf("Enter maximum Price: ");
    scanf("%f", &maximumPrice);
    for (Product *ptr = products; ptr < products + currentCount; ptr++)
    {
        if (ptr -> price >= minimumPrice && ptr -> price <= maximumPrice)
        {
            if (!found)
            {
                printf("\nProducts in price range:\n ");
            }
            printDetails(ptr);
            found = true;
        }
    }
    if (!found)
    {
        printf("\n No products found in the range %.2f and %.2f" , minimumPrice, maximumPrice);
    }
}

void deleteById(Product** products, int *currentCount)
{
    int productId;
    printf("Enter product ID to delete: ");
    scanf("%d", &productId);
    for (Product *ptr = *products; ptr < *products + *currentCount; ptr++)
    {
        if (ptr->id == productId)
        {
            for (Product *shiftPtr = ptr; shiftPtr < *products + (*currentCount - 1); shiftPtr++)
            {
                *shiftPtr = *(shiftPtr + 1);
            }
            (*currentCount)--;
            printf("\nProduct ID %d deleted successfully.\n", productId);
            Product *temp = realloc(*products, (*currentCount) * sizeof(Product));
            if (temp != NULL || *currentCount == 0)
            {
                *products = temp;
            }
            return;
        }
    }
    printf("\nProduct ID %d Not found.\n", productId);
}

int main()
{
    int capacity, currentCount = 0;
    printf("Enter initial product capacity: ");
    scanf("%d", &capacity);
    Product *products = (Product *)calloc(capacity, sizeof(Product));
    if (products == NULL)
    {
        printf("Memory allocation failed");
        return 1;
    }
    for (int index = 0; index < capacity; index++)
    {
        addProduct(&products, &capacity, &currentCount);
    }
    int choice;
    do
    {
        printMenu();
        printf("\nEnter your choice:");
        scanf("%d", &choice);
        switch ((MenuOptions)choice)
        {
            case ADD_PRODUCT:
                addProduct(&products, &capacity, &currentCount);
                break;
            case VIEW_PRODUCTS:
                displayProducts(products, currentCount);
                break;
            case UPDATE_QUANTITY:
                updateQuantity(products, currentCount);
                break;
            case SEARCH_BY_ID:
                searchProductById(products, currentCount);
                break;
            case SEARCH_BY_NAME:
                searchProductByName(products, currentCount);
                break;
            case SEARCH_BY_PRICE_RANGE:
                searchProductsByPriceRange(products, currentCount);
                break;
            case DELETE:
                deleteById(&products, &currentCount);
                break;
            case EXIT:
                printf("Memory released successfully. Exiting program...\n");
                break;
            default:
                printf("\nInvalid choice. Try again\n");
        }
    } while (choice != EXIT);
    free(products);
    products = NULL;
    return 0;
}
