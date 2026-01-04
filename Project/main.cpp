#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <cctype>        // for _stricmp
using namespace std;

const int MAX_PRODUCTS = 50;
const int MAX_INGREDIENTS = 10;
const int MAX_ALLERGIES = 20;

// Structure
struct Product {
    string pname;
    string ingredients[MAX_INGREDIENTS];
    int ingCount=0;
};
//Function Prototypes
void user_info(string& name, int& age);
void getallergies(string allergies[], int& allergycount);
void load_Product_Database(Product products[], int& productCount);
int Product_Safe(Product p, string allergies[], int allergyCount);
int Ingredient_Match(string ingredient, string allergies[], int allergyCount);
void filter_Safe_Products(Product products[], int productCount,
    string allergies[], int allergyCount,
    Product safeList[], int& safeCount);
void display_SafeProducts(Product safeList[], int safeCount);
void display_Summary(int total, int safe, int rejected);
int validate_Input(string input);
void saveUserProfile(string name, int age, string allergies[], int allergyCount);

// 1. User Info
void user_info(string& name, int& age) {
    cout << "\n___REGISTERATION___\n";
    cout << "Enter name: ";
    getline(cin, name);
    while (true) {
        cout << "Enter age (1-120): ";
        if (cin >> age && age >= 1 && age <= 120) {
            cin.ignore();
            break;
        }
        cout << "Invalid age!\n";
        cin.clear();
    }
    cout << "Profile created for " << name << "\n";
}

// 2. Get Allergies
void getallergies(string allergies[], int& allergycount) {
    cout << "___ALLERGY INFORMATION___\n";
    cout << "How many allergies? ";
    cin >> allergycount;
    cin.ignore();
    if (allergycount > MAX_ALLERGIES) allergycount = MAX_ALLERGIES;
    if (allergycount < 0) allergycount = 0;
    for (int i = 0; i < allergycount; i++) {
        cout << "Allergy " << i + 1 << ": ";
        getline(cin, allergies[i]);
    }
    cout << allergycount << " allergies recorded.\n";
}

// 3. Load Product Database - Modified with Pointer Arithmetic
void load_Product_Database(Product products[], int& productCount) {
    cout << "\n___LOADING DATABASE___\n";
    ifstream file("products.txt");
    if (!file) {
        cout << "ERROR: products.txt not found!\n";
        productCount = 0;
        return;
    }
    file >> productCount;
    file.ignore();

    for (int i = 0; i < productCount; i++) {
        // Using (products + i) which is a pointer to the i-th element
        Product* currentProduct = (products + i);

        getline(file, currentProduct->pname);
        file >> currentProduct->ingCount;
        file.ignore();

        for (int j = 0; j < currentProduct->ingCount; j++) {
            // Using pointer arithmetic to access the ingredients array within the struct
            getline(file, *(currentProduct->ingredients + j));
        }
    }
    file.close();
    cout << "SUCCESS: " << productCount << " products loaded.\n";
}


// 4. Product_Safe
int Product_Safe(Product p, string allergies[], int allergyCount) {
    if (allergyCount == 0) return 1;
    for (int i = 0; i < p.ingCount; i++) {
        for (int j = 0; j < allergyCount; j++) {
            if (_stricmp(p.ingredients[i].c_str(), allergies[j].c_str()) == 0)
                return 0;
        }
    }
    return 1;
}

// 5. Ingredient_Match
int Ingredient_Match(string ingredient, string allergies[], int allergyCount) {
    for (int j = 0; j < allergyCount; j++) {
        if (_stricmp(ingredient.c_str(), allergies[j].c_str()) == 0)
            return 1;
    }
    return 0;
}

// 6. filter_Safe_Products
void filter_Safe_Products(Product products[], int productCount,
    string allergies[], int allergyCount,
    Product safeList[], int& safeCount) {
    cout << "\n__COUNTING SAFE PRODUCTS__\n";
    safeCount = 0;
    for (int i = 0; i < productCount; i++) {
        if (Product_Safe(products[i], allergies, allergyCount)) {
            safeList[safeCount++] = products[i];
        }
    }
    cout << safeCount << " safe products found.\n";
}

// 7. display_SafeProducts
void display_SafeProducts(Product safeList[], int safeCount) {
    cout << "\n=====================";
    cout << "\n||SAFE PRODUCT LIST||\n";
    cout << "=====================\n";
    if (safeCount == 0) {
        cout << "No safe products found.\n";
        return;
    }
    for (int i = 0; i < safeCount; i++) {
        cout << i + 1 << ". " << safeList[i].pname << "\n   Ingredients: ";
        for (int j = 0; j < safeList[i].ingCount; j++) {
            cout << safeList[i].ingredients[j] << (j < safeList[i].ingCount - 1 ? ", " : "");
        }
        cout << endl;
    }
    cout << endl;
}

// 8. display_Summary
void display_Summary(int total, int safe, int rejected) {
    cout << "\n___SUMMARY REPORT___\n";
    cout << "Total products   : " << total << endl;
    cout << "Safe products    : " << safe << endl;
    cout << "Rejected         : " << rejected << endl;
    cout << "------------------------";
}

// 9. validate_Input
int validate_Input(string input) {
    if (input.empty()) {
        cout << "ERROR: Empty input!\n";
        return 0;
    }
    return 1;
}

// 10. saveUserProfile
void saveUserProfile(string name, int age, string allergies[], int allergyCount) {
    ofstream outFile("user_profile.txt");
    cout << "Name: " << name << "\nAge: " << age << "\nAllergies: ";
    for (int i = 0; i < allergyCount; i++) {
        outFile << allergies[i] << (i < allergyCount - 1 ? ", " : "");
    }
    outFile.close();
    cout << "\nProfile saved to user_profile.txt\n\n";
}


int main() {
    string name;
    int age = 0;

    // Using 'new' to allocate memory on the heap for large arrays
    string* allergies = new string[MAX_ALLERGIES];
    Product* products = new Product[MAX_PRODUCTS];
    Product* safeList = new Product[MAX_PRODUCTS];

    int allergyCount = 0;
    int productCount = 0;
    int safeCount = 0;

    user_info(name, age);
    getallergies(allergies, allergyCount);
    load_Product_Database(products, productCount);

    filter_Safe_Products(products, productCount, allergies, allergyCount, safeList, safeCount);
    display_SafeProducts(safeList, safeCount);
    display_Summary(productCount, safeCount, productCount - safeCount);
    saveUserProfile(name, age, allergies, allergyCount);

    // Clean up heap memory before exiting
    delete[] allergies;
    delete[] products;
    delete[] safeList;

    return 0;
}