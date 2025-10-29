#include <stdio.h>
#include <stdlib.h>

typedef struct product
{
    int productId;
    char productName[50];
    float price;
    int quantity;
} product;

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
} menuOptions;

void removeNewLine(char *str)
{
    for(char *character = str; *character != '\0'; character++)
    {
        if(*character == '\n')
        {
            *character = '\0';
            break;
        }
    }
}

int isSubstringExists(const char *original, const char *substring)
{
    int found = 0;
    if (*substring == '\0')
    {
        found = 1;
    }
    else
    {
        const char *ptrOriginal = original;
        while (*ptrOriginal != '\0' && !found)
        {
            const char *ptr = ptrOriginal;
            const char *ptrSubstring = substring;
            while (*ptr != '\0' && *ptrSubstring != '\0' && *ptr == *ptrSubstring)
            {
                ptr++;
                ptrSubstring++;
            }
            if (*ptrSubstring == '\0')
            {
                found = 1;
            }
            ptrOriginal++;
        }
    }
    return found;
}

void printDetails(product *product)
{
    printf("\nProduct ID : %d | Name : %s | Price : %.2f | Quantity : %d\n",
           product->productId,
           product->productName,
           product->price,
           product->quantity);
}

void addNewProducts(product** products, int *productCount, int *currentCount)
{
    if (*currentCount >= *productCount)
    {
        *productCount *= 2;
        product *temp = (product *)realloc(*products, (*productCount)*sizeof(product));
        if (temp != NULL)
        {
            *products = temp;
        }
    }
    product *p = *products + *currentCount;
    printf("\nEnter details for product %d: \n", *currentCount+1);
    printf("\nProduct Id : ");
    scanf("%d", &p->productId);
    getchar();
    printf("Product Name : ");
    fgets(p -> productName, sizeof(p -> productName), stdin);
    removeNewLine(p -> productName);
    printf("Product Price : ");
    scanf("%f", &p->price);
    printf("Product Quantity : ");
    scanf("%d", &p->quantity);
    (*currentCount)++;
    printf("\nproduct added successfully!\n");
}

void viewAllProducts(product *products, const int currentCount)
{
    int status = 1;
    if (currentCount == 0)
    {
        printf("No Product details found");
        status = 0;
    }
    if (status)
    {
        printf("\nPRODUCT LIST\n");
        for (product *ptr = products; ptr < products + currentCount; ptr++)
        {
            printDetails(ptr);
        }
    }
}

void updateQuantity(product *products, const int currentCount)
{
    int productId, newQuantity;
    printf("Enter Product ID to update quantity: ");
    scanf("%d", &productId);
    printf("Enter new Quantity: ");
    scanf("%d", &newQuantity);
    for (product *ptr = products; ptr < products + currentCount; ptr++)
    {
        if (ptr -> productId == productId)
        {
            ptr -> quantity = newQuantity;
            printf("\nQuantity updated successfully!");
            return;
        }
    }
    printf("\n Product ID %d not found.\n", productId);
}

void searchProductById(product *products, const int currentCount)
{
    int productId;
    printf("Enter Product ID search: ");
    scanf("%d", &productId);
    for (product *ptr = products; ptr < products + currentCount; ptr++)
    {
        if (ptr -> productId == productId)
        {
            printf("\nProduct found: ");
            printDetails(ptr);
            return;
        }
    }
    printf("\n Product ID %d not found.\n", productId);
}

void searchProductByName(product *products, const int currentCount)
{
    char searchName[50];
    int found = 0;
    getchar();
    printf("\nEnter product name to search: ");
    fgets(searchName, sizeof(searchName), stdin);
    removeNewLine(searchName);
    for (product *ptr = products; ptr < products + currentCount; ptr++)
    {
        if (isSubstringExists(ptr->productName, searchName))
        {
            if (!found)
            {
                printf("\nProducts found: \n");
            }
            printDetails(ptr);
            found = 1;
        }
    }
    if (!found)
    {
        printf("\n No products found with the name matching %s.\n", searchName);
    }
}

void searchProductsByPriceRange(product *products, const int currentCount)
{
    float minimumPrice, maximumPrice;
    int found = 0;
    printf("Enter minimum Price: ");
    scanf("%f", &minimumPrice);
    printf("Enter maximum Price: ");
    scanf("%f", &maximumPrice);
    for (product *ptr = products; ptr < products + currentCount; ptr++)
    {
        if (ptr -> price >= minimumPrice && ptr -> price <= maximumPrice)
        {
            if (!found)
            {
                printf("\nProducts in price range:\n ");
            }
            printDetails(ptr);
            found = 1;
        }
    }
    if (!found)
    {
        printf("\n No products found in the range %.2f and %.2f" , minimumPrice, maximumPrice);
    }
}

void deleteById(product** products, int *currentCount)
{
    int productId;
    printf("Enter product ID to delete: ");
    scanf("%d", &productId);
    for(product *ptr = *products; ptr < *products + *currentCount; ptr++)
    {
        if(ptr -> productId == productId)
        {
            
            for(product *shiftPtr = ptr; shiftPtr < *products + (*currentCount - 1); shiftPtr++)
            {
                *shiftPtr = *(shiftPtr + 1);
            }
            (*currentCount)--;
            printf("\nProduct ID %d deleted successfully.\n", productId);
            return;
        }
    }
    printf("\nProduct ID %d Not found.\n", productId);
}

int main()
{
    int productCount, result = 1, currentCount = 0;
    printf("Enter initial product count: ");
    scanf("%d", &productCount);
    product *products = (product *)calloc(productCount, sizeof(product));
    if (products == NULL)
    {
        printf("Memory allocation failed");
        result = 0;
    }
    for (int index = 0; index < productCount; index++)
    {
        addNewProducts(&products, &productCount, &currentCount);
    }
    if (result)
    {
        int choice;
        do
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
            printf("Enter your choice:");
            scanf("%d", &choice);

            switch ((menuOptions)choice)
            {
                case ADD_PRODUCT:
                    addNewProducts(&products, &productCount, &currentCount);
                    break;
                case VIEW_PRODUCTS:
                    viewAllProducts(products, currentCount);
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
        
    }
    free(products);
    return 0;
}
