#include <iostream>
#include <stack>
#include <queue>
#include <string>
#include <list>
#include <map>
using namespace std;

// Inventory Management Class
class ItemNode {
public:
    int ID;
    string name;
    string category;
    double buyPrice;
    double sellPrice;
    int quantity;
    int expire;
    string supplierName;
    int discount;
    int demand;
    ItemNode* next;
    ItemNode* left;
    ItemNode* right;

    // Constructor
    ItemNode(int ID, int quantity, int expire, int discount, double sellPrice, double buyPrice, string name, string category, string supplierName, int demand) {
        this->ID = ID;
        this->name = name;
        this->category = category;
        this->buyPrice = buyPrice;
        this->sellPrice = sellPrice;
        this->quantity = quantity;
        this->expire = expire;
        this->supplierName = supplierName;
        this->discount = discount;
        this->demand = demand;
        this->next = nullptr;
        this->left = nullptr;
        this->right = nullptr;
    }
};

class InventoryManagement {
private:
    ItemNode* head;
    ItemNode* tail;
    ItemNode* root;

public:
    // Constructor
    InventoryManagement() : head(nullptr), tail(nullptr), root(nullptr) {}

    // Add Product
    void addProduct(int ID, int quantity, int expire, int discount, double sellPrice, double buyPrice, string name, string category, string supplierName, int demand) {
        ItemNode* newItem = new ItemNode(ID, quantity, expire, discount, sellPrice, buyPrice, name, category, supplierName, demand);
        if (!head) {
            head = tail = newItem;
        } else {
            tail->next = newItem;
            tail = newItem;
        }
        cout << "Product added successfully: " << name << endl;
    }

    // Update Product
    void updateProduct(int ID, string name, string category, int quantity = 0, int expire = 0, int discount = 0, double sellPrice = 0, double buyPrice = 0, string supplierName = "", int demand = 0) {
        ItemNode* current = head;
        while (current) {
            if (current->ID == ID) {
                current->name = name;
                current->category = category;
                if (quantity) current->quantity = quantity;
                if (expire) current->expire = expire;
                if (discount) current->discount = discount;
                if (sellPrice) current->sellPrice = sellPrice;
                if (buyPrice) current->buyPrice = buyPrice;
                if (!supplierName.empty()) current->supplierName = supplierName;
                if (demand) current->demand = demand;
                cout << "Product updated successfully: " << name << endl;
                return;
            }
            current = current->next;
        }
        cout << "Product with ID " << ID << " not found.\n";
    }

    // Remove Product
    void removeProduct(int ID) {
        ItemNode* current = head;
        ItemNode* prev = nullptr;
        while (current) {
            if (current->ID == ID) {
                if (prev) {
                    prev->next = current->next;
                } else {
                    head = current->next;
                }
                if (current == tail) {
                    tail = prev;
                }
                delete current;
                cout << "Product removed successfully.\n";
                return;
            }
            prev = current;
            current = current->next;
        }
        cout << "Product with ID " << ID << " not found.\n";
    }

    // Categorize Product into BST
    void categorizeProduct(int ID) {
        ItemNode* current = head;
        while (current) {
            if (current->ID == ID) {
                root = insertToBST(root, current);
                cout << "Product categorized successfully: " << current->name << endl;
                return;
            }
            current = current->next;
        }
        cout << "Product with ID " << ID << " not found in inventory.\n";
    }

    ItemNode* insertToBST(ItemNode* node, ItemNode* product) {
        if (!node) return new ItemNode(*product);
        if (product->ID < node->ID) {
            node->left = insertToBST(node->left, product);
        } else {
            node->right = insertToBST(node->right, product);
        }
        return node;
    }

    // Search Product by Name, Category, or ID
    void searchProduct(int ID = 0, string name = "", string category = "") {
        if (root) {
            searchInBST(root, ID, name, category);
        } else {
            cout << "No products categorized yet.\n";
        }
    }

    void searchInBST(ItemNode* node, int ID, string name, string category) {
        if (!node) return;

        if ((ID && node->ID == ID) || (!name.empty() && node->name == name) || (!category.empty() && node->category == category)) {
            cout << "Product found: " << node->name << " (" << node->category << ")" << endl;
            cout << "ID: " << node->ID << ", Price: $" << node->sellPrice << ", Quantity: " << node->quantity << endl;
            return;
        }

        if (ID && ID < node->ID) {
            searchInBST(node->left, ID, name, category);
        } else {
            searchInBST(node->right, ID, name, category);
        }
    }

    // Generate Inventory Reports
    void generateReport() {
        if (!head) {
            cout << "Inventory is empty.\n";
            return;
        }

        ItemNode* current = head;
        priority_queue<pair<int, string>> lowStockQueue;

        cout << "\n--- Inventory Report ---\n";
        while (current) {
            cout << "Product: " << current->name << ", Category: " << current->category
                 << ", Quantity: " << current->quantity << ", Price: $" << current->sellPrice << endl;
            if (current->quantity < 5) {
                lowStockQueue.push({current->quantity, current->name});
            }
            current = current->next;
        }

        if (!lowStockQueue.empty()) {
            cout << "\n--- Low Stock Alerts ---\n";
            while (!lowStockQueue.empty()) {
                auto item = lowStockQueue.top();
                lowStockQueue.pop();
                cout << "Product: " << item.second << " has only " << item.first << " left in stock.\n";
            }
        }
    }
};

// Cart Item Class
class CartItem {
public:
    int ID;
    string name;
    double price;
    int quantity;
    double discount;

    CartItem(int ID, string name, double price, int quantity, double discount)
        : ID(ID), name(name), price(price), quantity(quantity), discount(discount) {}
};

// Shopping Cart Class
class ShoppingCart {
public:
    list<CartItem>cart;            // Linked list for dynamic cart storage
    stack<list<CartItem>> undoStack; // Stack for undo operations

public:
    // Add Item to Cart
    void addItem(int ID, string name, double price, int quantity, double discount) {
        cart.push_back(CartItem(ID, name, price, quantity, discount));
        undoStack.push(cart); // Save state for undo functionality
        cout << "Item added to cart: " << name << " (Quantity: " << quantity << ")\n";
    }

    // Remove Last Added Item
    void removeLastItem() {
        if (!cart.empty()) {
            cart.pop_back();
            undoStack.push(cart); // Save state
            cout << "Last item removed from cart.\n";
        } else {
            cout << "Cart is empty. Nothing to remove.\n";
        }
    }

    // Undo Last Operation
    void undoLastAction() {
        if (!undoStack.empty()) {
            undoStack.pop(); // Remove the last saved state
            cart = undoStack.empty() ? list<CartItem>() : undoStack.top();
            cout << "Undo successful. Current cart size: " << cart.size() << endl;
        } else {
            cout << "No actions to undo.\n";
        }
    }

    // Update Quantity of an Existing Item
    void updateQuantity(int ID, int newQuantity) {
        for (auto& item : cart) {
            if (item.ID == ID) {
                item.quantity = newQuantity;
                undoStack.push(cart); // Save state
                cout << "Quantity updated for item: " << item.name << " (New Quantity: " << newQuantity << ")\n";
                return;
            }
        }
        cout << "Item with ID " << ID << " not found in cart.\n";
    }

    // Display Cart Summary
    void displayCart(double taxRate = 0.1) {
        if (cart.empty()) {
            cout << "Cart is empty.\n";
            return;
        }

        double total = 0.0;
        cout << "\n--- Cart Summary ---\n";
        for (const auto& item : cart) {
            double itemTotal = item.price * item.quantity;
            double itemDiscount = itemTotal * (item.discount / 100);
            itemTotal -= itemDiscount;
            total += itemTotal;
            cout << "Item: " << item.name
                 << ", Quantity: " << item.quantity
                 << ", Price: $" << item.price
                 << ", Subtotal: $" << itemTotal
                 << " (Discount: " << item.discount << "%)\n";
        }

        double tax = total * taxRate;
        total += tax;
        cout << "Tax: $" << tax << "\n";
        cout << "Total Price: $" << total << "\n";
    }
};

// Order Class
class Order {
public:
    int orderID;
    list<CartItem> itemList;
    double totalPrice;

    Order(int orderID, const list<CartItem>& itemList, double totalPrice)
        : orderID(orderID), itemList(itemList), totalPrice(totalPrice) {}
};

// Checkout and Order Management Class
class CheckoutAndOrderManager {
private:
    queue<Order> orderQueue; // Queue for managing orders
    int nextOrderID;

public:
    // Constructor
    CheckoutAndOrderManager() : nextOrderID(1) {}

    // Checkout Process
    void checkout(list<CartItem>& cart) {
        if (cart.empty()) {
            cout << "Cart is empty. Nothing to checkout.\n";
            return;
        }

        double total = 0.0;
        cout << "\n--- Checkout ---\n";
        for (const auto& item : cart) {
            double itemTotal = item.price * item.quantity;
            total += itemTotal;
            cout << "Item: " << item.name
                 << ", Quantity: " << item.quantity
                 << ", Subtotal: $" << itemTotal << endl;
        }

        cout << "Total Price: $" << total << endl;

        // Place order in queue
        orderQueue.push(Order(nextOrderID++, cart, total));
        cart.clear(); // Clear the cart after checkout
        cout << "Order placed successfully. Order ID: " << nextOrderID - 1 << endl;
    }

    // Fulfill Order (FIFO)
    void fulfillOrder() {
        if (orderQueue.empty()) {
            cout << "No pending orders to fulfill.\n";
            return;
        }

        Order currentOrder = orderQueue.front();
        orderQueue.pop();

        cout << "\n--- Fulfilling Order ---\n";
        cout << "Order ID: " << currentOrder.orderID << "\nItems:\n";
        for (const auto& item : currentOrder.itemList) {
            cout << "Item: " << item.name
                 << ", Quantity: " << item.quantity
                 << ", Price: $" << item.price << endl;
        }
        cout << "Total Price: $" << currentOrder.totalPrice << "\n";
        cout << "Order fulfilled successfully.\n";
    }

    // Display Pending Orders
    void displayPendingOrders() {
        if (orderQueue.empty()) {
            cout << "No pending orders.\n";
            return;
        }

        cout << "\n--- Pending Orders ---\n";
        queue<Order> tempQueue = orderQueue;
        while (!tempQueue.empty()) {
            Order currentOrder = tempQueue.front();
            tempQueue.pop();
            cout << "Order ID: " << currentOrder.orderID
                 << ", Total Price: $" << currentOrder.totalPrice << "\n";
        }
    }
};

// Product Node for Analytics
class ProductNode {
public:
    string name;
    string category;
    int sales;
    int stock;

    ProductNode(string name, string category, int sales, int stock)
        : name(name), category(category), sales(sales), stock(stock) {}
};

// Analytics and Reporting Class
class AnalyticsAndReporting {
private:
    stack<string> systemLogs;           // Stack for logging operations
    priority_queue<pair<int, string>> lowStockQueue; // Priority queue for low-stock alerts
    map<string, int> salesByCategory;   // Map for sales by category

public:
    // Log System Operation
    void logOperation(const string& operation) {
        systemLogs.push(operation);
        cout << "Logged operation: " << operation << endl;
    }

    // Generate Sales Report by Product
    void generateSalesReportByProduct(map<string, int>& salesData) {
        cout << "\n--- Sales Report by Product ---\n";
        for (const auto& entry : salesData) {
            cout << "Product: " << entry.first << ", Sales: " << entry.second << endl;
        }
    }

    // Generate Sales Report by Category
    void generateSalesReportByCategory() {
        cout << "\n--- Sales Report by Category ---\n";
        for (const auto& entry : salesByCategory) {
            cout << "Category: " << entry.first << ", Sales: " << entry.second << endl;
        }
    }

    // Highlight Low Stock Items
    void addLowStockItem(const string& name, int stock) {
        lowStockQueue.push({-stock, name}); // Use negative stock for ascending order in priority queue
    }

    void displayLowStockAlerts() {
        if (lowStockQueue.empty()) {
            cout << "No low stock items.\n";
            return;
        }

        cout << "\n--- Low Stock Alerts ---\n";
        while (!lowStockQueue.empty()) {
            auto item = lowStockQueue.top();
            lowStockQueue.pop();
            cout << "Product: " << item.second << " has only " << -item.first << " left in stock.\n";
        }
    }

    // Display System Logs
    void displaySystemLogs() {
        if (systemLogs.empty()) {
            cout << "No system logs available.\n";
            return;
        }

        cout << "\n--- System Logs ---\n";
        while (!systemLogs.empty()) {
            cout << systemLogs.top() << endl;
            systemLogs.pop();
        }
    }

    // Track Sales and Add to Category
    void trackSalesByCategory(const string& category, int sales) {
        salesByCategory[category] += sales;
    }
};

// Product Node for Promotions
class PromotionNode {
public:
    int ID;
    string name;
    string category;
    double price;
    int quantity;
    double discount;

    PromotionNode(int ID, string name, string category, double price, int quantity, double discount)
        : ID(ID), name(name), category(category), price(price), quantity(quantity), discount(discount) {}
};

// Promotions and Discounts Class
class PromotionsAndDiscounts {
private:
    map<string, double> promoCodes; // Map to store valid promotional codes and discounts

public:
    // Constructor to Initialize Promotional Codes
    PromotionsAndDiscounts() {
        promoCodes["DISCOUNT10"] = 0.10;
        promoCodes["DISCOUNT20"] = 0.20;
        promoCodes["DISCOUNT30"] = 0.30;
    }

    // Apply Discount Based on Demand
    void applyDynamicPricing(PromotionNode& product, int demand) {
        if (demand > 50) {
            product.price *= 1.20; // Increase price by 20% for high demand
            cout << "Dynamic pricing applied: New price for " << product.name << " is $" << product.price << endl;
        } else if (demand < 10) {
            product.price *= 0.90; // Decrease price by 10% for low demand
            cout << "Dynamic pricing applied: New price for " << product.name << " is $" << product.price << endl;
        } else {
            cout << "No dynamic pricing applied for " << product.name << ".\n";
        }
    }

    // Apply Seasonal Discounts
    void applySeasonalDiscount(PromotionNode& product, const string& season) {
        if (season == "Summer" && product.category == "Drinks") {
            product.price *= 0.85; // 15% off on drinks in summer
            cout << "Seasonal discount applied: New price for " << product.name << " is $" << product.price << endl;
        } else if (season == "Winter" && product.category == "Heaters") {
            product.price *= 0.80; // 20% off on heaters in winter
            cout << "Seasonal discount applied: New price for " << product.name << " is $" << product.price << endl;
        } else {
            cout << "No seasonal discount applied for " << product.name << ".\n";
        }
    }

    // Apply Promotional Code
    void applyPromoCode(PromotionNode& product, const string& promoCode) {
        if (promoCodes.find(promoCode) != promoCodes.end()) {
            double discountRate = promoCodes[promoCode];
            product.price *= (1 - discountRate);
            cout << "Promo code applied: New price for " << product.name << " is $" << product.price << endl;
        } else {
            cout << "Invalid promo code: " << promoCode << endl;
        }
    }
};

// Customer Profile Class
class CustomerProfile {
public:
    int customerID;
    string name;
    int rewardPoints;
    string membershipLevel;

    CustomerProfile(int customerID, string name, int rewardPoints)
        : customerID(customerID), name(name), rewardPoints(rewardPoints) {
        updateMembershipLevel();
    }

    // Update Membership Level Based on Reward Points
    void updateMembershipLevel() {
        if (rewardPoints >= 500) {
            membershipLevel = "Platinum";
        } else if (rewardPoints >= 300) {
            membershipLevel = "Gold";
        } else if (rewardPoints >= 100) {
            membershipLevel = "Silver";
        } else {
            membershipLevel = "Bronze";
        }
    }
};

// Loyalty Program Class
class LoyaltyProgram {
private:
    list<CustomerProfile> customerProfiles; // Linked list to store customer profiles

public:
    // Add New Customer Profile
    void addCustomerProfile(int customerID, string name) {
        customerProfiles.push_back(CustomerProfile(customerID, name, 0));
        cout << "Customer profile added: " << name << " (ID: " << customerID << ")\n";
    }

    // Update Reward Points
    void updateRewardPoints(int customerID, int points) {
        for (auto& profile : customerProfiles) {
            if (profile.customerID == customerID) {
                profile.rewardPoints += points;
                profile.updateMembershipLevel();
                cout << "Updated reward points for " << profile.name
                     << ". Total Points: " << profile.rewardPoints
                     << ", Membership Level: " << profile.membershipLevel << "\n";
                return;
            }
        }
        cout << "Customer with ID " << customerID << " not found.\n";
    }

    // Display All Customer Profiles
    void displayAllProfiles() {
        if (customerProfiles.empty()) {
            cout << "No customer profiles found.\n";
            return;
        }

        cout << "\n--- Customer Profiles ---\n";
        for (const auto& profile : customerProfiles) {
            cout << "Customer ID: " << profile.customerID
                 << ", Name: " << profile.name
                 << ", Reward Points: " << profile.rewardPoints
                 << ", Membership Level: " << profile.membershipLevel << "\n";
        }
    }

    // Provide Exclusive Offers
    void provideExclusiveOffers() {
        if (customerProfiles.empty()) {
            cout << "No customers to provide offers.\n";
            return;
        }

        cout << "\n--- Exclusive Offers ---\n";
        for (const auto& profile : customerProfiles) {
            if (profile.membershipLevel == "Platinum") {
                cout << profile.name << ": 20% off on all purchases.\n";
            } else if (profile.membershipLevel == "Gold") {
                cout << profile.name << ": 15% off on all purchases.\n";
            } else if (profile.membershipLevel == "Silver") {
                cout << profile.name << ": 10% off on all purchases.\n";
            }
        }
    }
};
class AdminWorkflow {
private:
    InventoryManagement& inventoryManager;
    AnalyticsAndReporting& analytics;
    PromotionsAndDiscounts& promotions;

public:
    AdminWorkflow(InventoryManagement& inventory, AnalyticsAndReporting& analytics, PromotionsAndDiscounts& promotions)
        : inventoryManager(inventory), analytics(analytics), promotions(promotions) {}

    void start() {
        int choice;
        do {
            cout << "\n--- Admin Menu ---\n";
            cout << "1. Add Product\n";
            cout << "2. Update Product\n";
            cout << "3. Remove Product\n";
            cout << "4. Categorize Product\n";
            cout << "5. Search Product\n";
            cout << "6. Generate Inventory Report\n";
            cout << "7. Apply Dynamic Pricing\n";
            cout << "8. Display System Logs\n";
            cout << "0. Logout\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    int ID, quantity, expire, discount, demand;
                    double sellPrice, buyPrice;
                    string name, category, supplierName;
                    cout << "Enter product details:\n";
                    cout << "ID: "; cin >> ID;
                    cout << "Name: "; cin >> name;
                    cout << "Category: "; cin >> category;
                    cout << "Buy Price: "; cin >> buyPrice;
                    cout << "Sell Price: "; cin >> sellPrice;
                    cout << "Quantity: "; cin >> quantity;
                    cout << "Expire (days): "; cin >> expire;
                    cout << "Supplier Name: "; cin >> supplierName;
                    cout << "Discount (%): "; cin >> discount;
                    cout << "Demand: "; cin >> demand;
                    inventoryManager.addProduct(ID, quantity, expire, discount, sellPrice, buyPrice, name, category, supplierName, demand);
                    analytics.logOperation("Product added: " + name);
                    break;
                }
                case 2: {
                    int ID, quantity, discount;
                    double sellPrice, buyPrice;
                    string name, category, supplierName;
                    cout << "Enter product ID to update: "; cin >> ID;
                    cout << "New Name: "; cin >> name;
                    cout << "New Category: "; cin >> category;
                    cout << "New Quantity: "; cin >> quantity;
                    cout << "New Expiry (days): "; cin >> discount;
                    cout << "New Sell Price: "; cin >> sellPrice;
                    cout << "New Buy Price: "; cin >> buyPrice;
                    cout << "New Supplier Name: "; cin >> supplierName;
                    inventoryManager.updateProduct(ID, name, category, quantity, discount, 0, sellPrice, buyPrice, supplierName);
                    analytics.logOperation("Product updated: " + name);
                    break;
                }
                case 3: {
                    int ID;
                    cout << "Enter product ID to remove: "; cin >> ID;
                    inventoryManager.removeProduct(ID);
                    analytics.logOperation("Product removed with ID: " + to_string(ID));
                    break;
                }
                case 4: {
                    int ID;
                    cout << "Enter product ID to categorize: "; cin >> ID;
                    inventoryManager.categorizeProduct(ID);
                    analytics.logOperation("Product categorized with ID: " + to_string(ID));
                    break;
                }
                case 5: {
                    string name, category;
                    cout << "Enter product name (or leave blank): "; cin.ignore(); getline(cin, name);
                    cout << "Enter category (or leave blank): "; getline(cin, category);
                    inventoryManager.searchProduct(0, name, category);
                    break;
                }
                case 6:
                    inventoryManager.generateReport();
                    break;
                case 7: {
                    PromotionNode product(1, "Sample Product", "Category", 100.0, 50, 0);
                    promotions.applyDynamicPricing(product, 60);
                    break;
                }
                case 8:
                    analytics.displaySystemLogs();
                    break;
                case 0:
                    cout << "Logged out successfully.\n";
                    break;
                default:
                    cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 0);
    }
};
 class CustomerWorkflow {
private:
    ShoppingCart& shoppingCart;
    CheckoutAndOrderManager& orderManager;
    LoyaltyProgram& loyaltyProgram;

public:
    CustomerWorkflow(ShoppingCart& cart, CheckoutAndOrderManager& orderManager, LoyaltyProgram& loyaltyProgram)
        : shoppingCart(cart), orderManager(orderManager), loyaltyProgram(loyaltyProgram) {}

    void start() {
        int choice;
        do {
            cout << "\n--- Customer Menu ---\n";
            cout << "1. Add Item to Cart\n";
            cout << "2. Remove Last Item\n";
            cout << "3. Undo Last Action\n";
            cout << "4. Update Item Quantity\n";
            cout << "5. Display Cart\n";
            cout << "6. Checkout\n";
            cout << "7. View Loyalty Profile\n";
            cout << "8. Redeem Rewards\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    int ID, quantity;
                    double price;
                    string name;
                    cout << "Enter item details:\n";
                    cout << "ID: "; cin >> ID;
                    cout << "Name: "; cin >> name;
                    cout << "Price: "; cin >> price;
                    cout << "Quantity: "; cin >> quantity;
                    shoppingCart.addItem(ID, name, price, quantity, 0);
                    break;
                }
                case 2:
                    shoppingCart.removeLastItem();
                    break;
                case 3:
                    shoppingCart.undoLastAction();
                    break;
                case 4: {
                    int ID, newQuantity;
                    cout << "Enter item ID to update: "; cin >> ID;
                    cout << "Enter new quantity: "; cin >> newQuantity;
                    shoppingCart.updateQuantity(ID, newQuantity);
                    break;
                }
                case 5:
                    shoppingCart.displayCart();
                    break;
                case 6:
                    orderManager.checkout(shoppingCart.cart);
                    break;
                case 7:
                    loyaltyProgram.displayAllProfiles();
                    break;
                case 8:
                    loyaltyProgram.provideExclusiveOffers();
                    break;
                case 0:
                    cout << "Thank you for using the system. Goodbye!\n";
                    break;
                default:
                    cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 0);
    }
};
int main() {
    InventoryManagement inventoryManager;
    ShoppingCart shoppingCart;
    CheckoutAndOrderManager orderManager;
    AnalyticsAndReporting analytics;
    PromotionsAndDiscounts promotions;
    LoyaltyProgram loyaltyProgram;

    int userType;
    cout << "Welcome to the Supermarket Management System\n";
    cout << "Are you an:\n1. Admin\n2. Customer\nChoose an option: ";
    cin >> userType;

    if (userType == 1) {
        AdminWorkflow adminWorkflow(inventoryManager, analytics, promotions);
        adminWorkflow.start();
    } else if (userType == 2) {
        // Add a sample customer profile for demonstration
        loyaltyProgram.addCustomerProfile(1, "John Doe");
        CustomerWorkflow customerWorkflow(shoppingCart, orderManager, loyaltyProgram);
        customerWorkflow.start();
    } else {
        cout << "Invalid user type. Exiting the system.\n";
    }

    return 0;
}
