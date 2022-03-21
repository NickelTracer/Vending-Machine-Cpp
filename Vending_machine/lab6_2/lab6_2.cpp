#include <iostream>
#include <string>
#include <fstream>
using namespace std;
bool running = true;

/* Class of products */
class Products {

public:
    int id;
    int quantity;
    double price;

    Products(int id, int quantity, double price) {
        this->id = id;
        this->quantity = quantity;
        this->price = price;
    }
};

/* Class of coins */
class Coins {

public:
    int coin_value;
    int coin_ammount;
    string coin_type;

    Coins(int coin_value, int coin_ammount, string coin_type) {
        this->coin_value = coin_value;
        this->coin_ammount = coin_ammount;
        this->coin_type = coin_type;
    }
};

//update list of products from last run with use of .txt
void list_update(Products products[]) {
    int count = 0;
    int size = 5;

    ifstream inputFile;
    inputFile.open("list_of_products.txt");

    while (count<size && inputFile >> products[count].quantity) {
        count++;
    }

    inputFile.close();
}

//update list of coins from last run with use of .txt
void number_of_coins_update(Coins coins[]) {
    int count = 0;
    int size = 6;

    ifstream inputFile;
    inputFile.open("Number_of_coins.txt");

    while (count<size && inputFile >> coins[count].coin_ammount) {
        count++;
    }

    inputFile.close();
}

//display list of products and their quantity
void display_products(Products products[]) {

    for (int i = 0; i < 5; i++) {
        std::cout << products[i].id << ". prod" << products[i].id << ": " << products[i].quantity << "\t--> price: " << products[i].price << std::endl;
    }
}

//select product - hidden key 0000 - replenish coins 9999  
void product_selection(Products products[], Coins coins[], int &prod, int &prod_sel) {
    int amount_of_coins[6];

    cout << "Please select your product: "<<endl;
    cin >> prod;
    if (prod > 0 && prod <= 5) {
        prod_sel = 1;
    }
    else if (prod > 5 && prod != 9999) {
        prod_sel = 0;
    }
    else if (prod == 0000) {
        exit(1);
    }
    else if (prod == 9999) {
        cout << "Please refill amount of coins!" << endl << endl;
        cout << "Please insert amount of coins: " << endl;
        for (int i = 0; i < 6; i++) {
            cout << coins[i].coin_type<<": (current:"<<coins[i].coin_ammount<<") ";
            cin >> amount_of_coins[i];
            coins[i].coin_ammount = amount_of_coins[i] + coins[i].coin_ammount;
        }

        ofstream ofs{ "Number_of_coins.txt" };
        auto cout_buff = cout.rdbuf();
        cout.rdbuf(ofs.rdbuf());

        for (int i = 0; i < 6; i++) {
            cout << coins[i].coin_ammount << endl;
        }

        cout.rdbuf(cout_buff);

        exit(1);
    }
}

//confirm your selection
void confirmation(int &sel, int &kill, int &prod_sel) {

    cout << "Confirm your selection? (1) (0)";
    cin >> sel;

    switch (sel) {
    case 1:
        if (prod_sel == 0) {
            kill = 1;
        }
        break;
    case 0:
        cout << "Select your product again!" << endl;
        break;
    default:
        cout << "Wrong number entered! Please try again." << endl;
        break;
    }
}

//if wrong integer entered
void neg_sel(int &prod_sel, bool &running) {
    
    if (prod_sel == 0) {
        cout << "Wrong integer entered!";
        running = false;
    }
}

//subtract product amount, update .txt document
void product_amount(Products products[], int& sel, int& amount, int& prod) {
    if (sel == 1) {
        cout << "Please enter amount: ";
        cin >> amount;
        cout << endl;

        if (amount > products[prod - 1].quantity) {
            cout << "Wrong amount entered!" << endl;
            running = false;
        }
    }
}

//insert coins, show price
void insert_coins(Products products[], Coins coins[], double &credit_requested, int &amount, int &prod, double &to_return) {
    int amount_of_coins[6];
    int res;
    double credit = 0;

    credit_requested = amount * products[prod - 1].price;
    cout << "Please pay: " << credit_requested << "zl" << endl << endl;

    credit_requested = credit_requested * 100;

    for (int i = 0; i < 6; i++) {
        if (coins[i].coin_ammount <= 2 && coins[i].coin_ammount != 0) {
            cout << "Very low " << coins[i].coin_type << " coins, please pay with deducted coins!" << endl;
        }
        else if (coins[i].coin_ammount == 0) {
            cout << "No " << coins[i].coin_type << " coins, please pay with deducted coins!" << endl << endl;
        }
    }
    
    cout << "Please insert coins: " << endl;
    for (int i = 0; i < 6; i++) {
        cout << coins[i].coin_type << ": ";
        cin >> amount_of_coins[i];
        res = amount_of_coins[i] * coins[i].coin_value;
        credit += res;
    }

    for (int i = 0; i < 6; i++) {
        coins[i].coin_ammount = coins[i].coin_ammount + amount_of_coins[i];
    }

    cout << endl << "Money entered: " << credit / 100 << "zl";

    if (credit_requested <= credit) {
        to_return = credit - credit_requested;
        cout << endl << "Money to be returned: " << to_return / 100 << "zl" << endl;
    }
    else if (credit_requested > credit) {
        cout << "you need to pay more!" << endl;
        exit(1);
    }
}

//pay rest
void money_return(Coins coins[], double &to_return) {
    int threshold = 10;

    if (to_return > 0) {
        for (int i = 0; i < 6; i++) {
            while (coins[i].coin_ammount > threshold && (to_return - coins[i].coin_value) >= 0) {
                to_return -= coins[i].coin_value;
                coins[i].coin_ammount -= 1;
                cout << "Returning: " << coins[i].coin_type << endl;
            }
        }

        while ((to_return - 500) >= 0 && coins[0].coin_ammount - 1 > 0) {
            to_return -= 500;
            coins[0].coin_ammount -= 1;
            cout << "Returning: " << coins[0].coin_type << endl;
        }
        while ((to_return - 200) >= 0 && coins[1].coin_ammount - 1 > 0) {
            to_return -= 200;
            coins[1].coin_ammount -= 1;
            cout << "Returning: " << coins[1].coin_type << endl;
        }
        while ((to_return - 100) >= 0 && coins[2].coin_ammount - 1 > 0) {
            to_return -= 100;
            coins[2].coin_ammount -= 1;
            cout << "Returning: " << coins[2].coin_type << endl;
        }
        while ((to_return - 50) >= 0 && coins[3].coin_ammount - 1 > 0) {
            to_return -= 50;
            coins[3].coin_ammount -= 1;
            cout << "Returning: " << coins[3].coin_type << endl;
        }
        while ((to_return - 20) >= 0 && coins[4].coin_ammount - 1 > 0) {
            to_return -= 20;
            coins[4].coin_ammount -= 1;
            cout << "Returning: " << coins[4].coin_type << endl;
        }
        while ((to_return - 10) >= 0 && coins[5].coin_ammount - 1 > 0) {
            to_return -= 10;
            coins[5].coin_ammount -= 1;
            cout << "Returning: " << coins[5].coin_type << endl;
        }
    }

    if (to_return > 0.001) {
        cout << "Machine not able to spend rest! Pay with deducted cash or try again later!" << endl;
        exit(1);
    }
    cout << "Thank you for your purchase!" << endl;
    running = false;
}

//update list of coins to .txt document
void update_list_of_coins_and_products(Products products[], Coins coins[], int& amount, int& prod) {
    ofstream ofs{ "Number_of_coins.txt" };
    auto cout_buff = cout.rdbuf();
    cout.rdbuf(ofs.rdbuf());

    for (int i = 0; i < 6; i++) {
        cout << coins[i].coin_ammount << endl;
    }

    cout.rdbuf(cout_buff);

    if (amount <= products[prod - 1].quantity) {
        products[prod - 1].quantity = products[prod - 1].quantity - amount;

        ofstream ofs{ "list_of_products.txt" };
        auto cout_buff = cout.rdbuf();
        cout.rdbuf(ofs.rdbuf());

        for (int i = 0; i < 5; i++) {
            cout << products[i].quantity << endl;
        }

        cout.rdbuf(cout_buff);
        running = false;
    }
    running = false;
}

//update .txt document if replenish will occur
void replenish_product(Products products[], int& prod) {

    cout << "prod" << products[prod - 1].id << " is out of stock! Replenish product amount..." << endl << endl;
    products[prod - 1].quantity = 10;

    ofstream ofs{ "list_of_products.txt" };
    auto cout_buff = cout.rdbuf();
    cout.rdbuf(ofs.rdbuf());

    for (int i = 0; i < 5; i++) {
        cout << products[i].quantity << endl;
    }

    cout.rdbuf(cout_buff);
}

//.txt to update number of replenishes
void replenish_product_text_file(int& prod) {
    int quantity[5] = { 0,0,0,0,0 };
    int count = 0;
    int size = 5;

    ifstream inputFile;
    inputFile.open("resets.txt");

    while (count<size && inputFile >> quantity[count]) {
        count++;
    }
    quantity[prod - 1]++;

    ofstream ofs{ "resets.txt" };
    auto cout_buff = cout.rdbuf();
    cout.rdbuf(ofs.rdbuf());

    for (int i = 0; i < 5; i++) {
        cout << quantity[i] << endl;
    }

    cout.rdbuf(cout_buff);
}



int main() {
    int kill;
    int sel;
    int prod ;
    int prod_sel;
    int amount;

    double credit_requested;
    double to_return;

    Products prod1(1, 9, 2.5), prod2(2, 8, 6.5), prod3(3, 7, 1.9), prod4(4, 6, 3.9), prod5(5, 5, 4.2);
    Products products[5] = { prod1,prod2,prod3,prod4,prod5 };

    Coins coin5(500, 18, "5zl"), coin2(200, 17, "2zl"), coin1(100, 16, "1zl"), coin05(50, 15, "0.5zl"), coin02(20, 14, "0.2zl"), coin01(10, 13, "0.1zl");
    Coins coins[6] = { coin5, coin2, coin1, coin05, coin02, coin01 };

    cout << "Welcome to vending machine, avaiable products: " << endl << endl;
    list_update(products);
    number_of_coins_update(coins);
    display_products(products);

   while (running) {
       product_selection(products, coins, prod, prod_sel);
       confirmation(sel, kill, prod_sel);
       if (sel == 1) {
           if (kill == 1) {
               neg_sel(prod_sel, running);
           }
           else {
               product_amount(products, sel, amount, prod);
               if (amount <= products[prod - 1].quantity) {
                   insert_coins(products, coins, credit_requested, amount, prod, to_return);
                   money_return(coins, to_return);
                   update_list_of_coins_and_products(products, coins, amount, prod);
               }
               display_products(products);
               if (products[prod - 1].quantity == 0) {
                   replenish_product(products, prod);
                   replenish_product_text_file(prod);
                   running = false;
               }
               else {
                   running = false;
               }
           }
       }
    }
   return 0;
}