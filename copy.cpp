#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

class Pharmacy; // Forward declaration

void clearScreen()
{
    cout << "Press 1 to continue.....\n";
    int temp;
    cin >> temp;
    if (temp == 1)
    {
        system("cls");
    }
}

void adminMenu()
{
    clearScreen();
    cout << "=== Admin Authorized Menu ===\n";
    cout << "1. Admin Data\n";
    cout << "2. Medicine Data\n";
    cout << "3. Logout\n";
    cout << "Enter your choice: \n";
}

void customerMenu()
{
    cout << "=== Customer Menu ===\n";
    cout << "1. View Medications\n";
    cout << "2. Search Medications\n";
    cout << "3. Purchase Medications\n";
    cout << "4. View Order History\n";
    cout << "5. Logout\n";
    cout << "Enter your choice: ";
}

class Admin
{
public:
    virtual void addAdmin(Pharmacy &pharmacy, const string &newAdmin, const string &newPassword) = 0;
    virtual void deleteAdmin(Pharmacy &pharmacy, const string &adminToDelete) = 0;
    virtual void editAdmin(Pharmacy &pharmacy, const string &adminToEdit, const string &newPassword) = 0;
    virtual void previewAllAdmins(const Pharmacy &pharmacy) const = 0;
    virtual ~Admin() {}
};

class SuperAdmin : public Admin
{
public:
    void addAdmin(Pharmacy &pharmacy, const string &newAdmin, const string &newPassword) override;
    void deleteAdmin(Pharmacy &pharmacy, const string &adminToDelete) override;
    void editAdmin(Pharmacy &pharmacy, const string &adminToEdit, const string &newPassword) override;
    void previewAllAdmins(const Pharmacy &pharmacy) const override;
};

class SubAdmin : public Admin
{
public:
    void addAdmin(Pharmacy &pharmacy, const string &newAdmin, const string &newPassword) override;
    void deleteAdmin(Pharmacy &pharmacy, const string &adminToDelete) override;
    void editAdmin(Pharmacy &pharmacy, const string &adminToEdit, const string &newPassword) override;
    void previewAllAdmins(const Pharmacy &pharmacy) const override;
};

class Medication
{
private:
    string name;
    int quantity;
    double price;
    string manufacturer;

public:
    Medication(const string &name, int quantity, double price, const string &manufacturer)
        : name(name), quantity(quantity), price(price), manufacturer(manufacturer) {}

    string getName() const
    {
        return name;
    }

    int getQuantity() const
    {
        return quantity;
    }

    double getPrice() const
    {
        return price;
    }

    void setQuantity(int newQuantity)
    {
        quantity = newQuantity;
    }

    void display() const
    {
        cout << "Name: " << name << "\tQuantity: " << quantity << "\tPrice: $" << price
             << "\tManufacturer: " << manufacturer << endl;
    }
};

class Authentication
{
private:
    const string filename = "user.authentication.txt";

public:
    bool authenticateAdmin(const string &username, const string &password) const
    {
        ifstream file(filename);

        if (!file.is_open())
        {
            cout << "Error opening file for reading." << endl;
            return false;
        }

        string storedUsername, storedPassword;

        while (file >> storedUsername >> storedPassword)
        {
            if (username == storedUsername && password == storedPassword)
            {
                file.close();
                return true;
            }
        }

        file.close();

        return false;
    }
};

class Pharmacy
{
public:
    list<pair<string, string>> admins; // Pair of username and password
    list<Medication> medicines;

    Pharmacy();
    void readAdminsFromFile();
    void readMedicinesFromFile();
    void displayMedicines() const;
    void displayMedicationsForCustomer() const;

    void addAdmin(const string &newAdmin, const string &newPassword);
    void deleteAdmin(const string &adminToDelete);
    void editAdmin(const string &adminToEdit, const string &newPassword);
    void previewAllAdmins() const;
};

Pharmacy::Pharmacy()
{
    readAdminsFromFile();
    readMedicinesFromFile();
}

void Pharmacy::readAdminsFromFile()
{
    ifstream adminFile("user.authentication.txt");

    if (adminFile.is_open())
    {
        string adminUsername, adminPassword;

        while (adminFile >> adminUsername >> adminPassword)
        {
            admins.push_back(make_pair(adminUsername, adminPassword));
        }

        adminFile.close();
    }
    else
    {
        cerr << "Error opening admins file for reading." << endl;
    }
}

void Pharmacy::readMedicinesFromFile()
{
    ifstream medicineFile("medicines.txt");

    if (medicineFile.is_open())
    {
        string name;
        int quantity;
        double price;
        string manufacturer;

        while (medicineFile >> name >> quantity >> price >> manufacturer)
        {
            Medication newMedication(name, quantity, price, manufacturer);
            medicines.push_back(newMedication);
        }

        medicineFile.close();
    }
    else
    {
        cerr << "Error opening medicines file for reading." << endl;
    }
}

void Pharmacy::displayMedicines() const
{
    cout << "=== Pharmacy Inventory ===" << endl;
    for (const auto &medication : medicines)
    {
        medication.display();
        cout << "----------------------" << endl;
    }
}

void Pharmacy::displayMedicationsForCustomer() const
{
    cout << "=== Available Medications ===" << endl;
    for (const auto &medication : medicines)
    {
        cout << "Name: " << medication.getName() << "\tPrice: $" << medication.getPrice() << endl;
    }
}

void Pharmacy::addAdmin(const string &newAdmin, const string &newPassword)
{
    admins.push_back(make_pair(newAdmin, newPassword));

    ofstream adminFile("user.authentication.txt", ios::app);
    if (adminFile.is_open())
    {
        adminFile << newAdmin << " " << newPassword << "\n";
        adminFile.close();
        cout << "Admin added successfully.\n";
    }
    else
    {
        cerr << "Error opening file for writing.\n";
    }
}

void Pharmacy::deleteAdmin(const string &adminToDelete)
{
    auto it = find_if(admins.begin(), admins.end(),
                      [adminToDelete](const auto &admin)
                      { return admin.first == adminToDelete; });

    if (it != admins.end())
    {
        admins.erase(it);

        ofstream adminFile("user.authentication.txt");
        if (adminFile.is_open())
        {
            for (const auto &admin : admins)
            {
                adminFile << admin.first << " " << admin.second << "\n";
            }
            adminFile.close();
            cout << "Admin deleted successfully.\n";
        }
        else
        {
            cerr << "Error opening file for writing.\n";
        }
    }
    else
    {
        cout << "Admin not found.\n";
    }
}

void Pharmacy::editAdmin(const string &adminToEdit, const string &newPassword)
{
    auto it = find_if(admins.begin(), admins.end(),
                      [adminToEdit](const auto &admin)
                      { return admin.first == adminToEdit; });

    if (it != admins.end())
    {
        it->second = newPassword;

        ofstream adminFile("user.authentication.txt");
        if (adminFile.is_open())
        {
            for (const auto &admin : admins)
            {
                adminFile << admin.first << " " << admin.second << "\n";
            }
            adminFile.close();
            cout << "Admin edited successfully.\n";
        }
        else
        {
            cerr << "Error opening file for writing.\n";
        }
    }
    else
    {
        cout << "Admin not found.\n";
    }
}

// Implementation of the Preview function in the Pharmacy class
void Pharmacy::previewAllAdmins() const
{
    cout << "=== All Admin Details ===\n";
    for (const auto &admin : admins)
    {
        cout << "Username: " << admin.first << "\n";
        cout << "Password: " << admin.second << "\n";
        cout << "----------------------\n";
    }
}

class Customer
{
private:
    static int nextCustomerNumber;
    int customerNumber;
    string name;
    int orderID;

public:
    Customer(const string &name) : name(name)
    {
        customerNumber = generateUniqueCustomerNumber();
    }

    string getName() const
    {
        return name;
    }

    int getOrderID() const
    {
        return orderID;
    }

    void viewMedications(const Pharmacy &pharmacy) const
    {
        pharmacy.displayMedicationsForCustomer();
    }

    void searchMedications(const Pharmacy &pharmacy) const
    {
        string searchKeyword;
        cout << "Enter the name of the medication to search: ";
        cin.ignore();
        getline(cin, searchKeyword);

        bool found = false;

        cout << "=== Search Results ===" << endl;

        for (const auto &medication : pharmacy.medicines)
        {
            if (medication.getName().find(searchKeyword) != string::npos)
            {
                medication.display();
                found = true;
                cout << "----------------------" << endl;
            }
        }

        if (!found)
        {
            cout << "No medications found with the given name.\n";
        }
    }

    void purchaseMedications(Pharmacy &pharmacy)
    {
        string purchaseName;
        int purchaseQuantity;

        cout << "Enter the name of the medication you want to purchase: ";
        cin.ignore();
        getline(cin, purchaseName);

        auto it = find_if(pharmacy.medicines.begin(), pharmacy.medicines.end(),
                          [purchaseName](const auto &medication)
                          { return medication.getName() == purchaseName; });

        if (it != pharmacy.medicines.end())
        {
            cout << "Enter the quantity you want to purchase: ";
            cin >> purchaseQuantity;

            if (purchaseQuantity > 0 && purchaseQuantity <= it->getQuantity())
            {
                it->setQuantity(it->getQuantity() - purchaseQuantity);
                cout << "Purchase successful!\n";

                orderID = rand();

                string orderFileName = "order_" + to_string(orderID) + ".txt";
                ofstream orderFile(orderFileName);
                if (orderFile.is_open())
                {
                    orderFile << "Order Details\n";
                    orderFile << "Customer Name: " << name << "\n";
                    orderFile << "Medication Name: " << purchaseName << "\n";
                    orderFile << "Quantity: " << purchaseQuantity << "\n";
                    orderFile << "Total Price: $" << (purchaseQuantity * it->getPrice()) << "\n";
                    orderFile << "------------------------\n";
                    orderFile.close();
                    cout << "Order details stored in " << orderFileName << ".\n";
                }
                else
                {
                    cerr << "Error opening file for writing.\n";
                }
            }
            else
            {
                cout << "Invalid quantity. Purchase failed.\n";
            }
        }
        else
        {
            cout << "Medication not found. Purchase failed.\n";
        }
    }

    void viewOrderHistory() const
    {
        cout << "Viewing order history...\n";
        // Implement order history functionality as needed
    }

private:
    static int generateUniqueCustomerNumber()
    {
        return nextCustomerNumber++;
    }
};

// Implementation of the Admin virtual functions in the SuperAdmin class
void SuperAdmin::addAdmin(Pharmacy &pharmacy, const string &newAdmin, const string &newPassword)
{
    pharmacy.addAdmin(newAdmin, newPassword);
}

void SuperAdmin::deleteAdmin(Pharmacy &pharmacy, const string &adminToDelete)
{
    pharmacy.deleteAdmin(adminToDelete);
}

void SuperAdmin::editAdmin(Pharmacy &pharmacy, const string &adminToEdit, const string &newPassword)
{
    pharmacy.editAdmin(adminToEdit, newPassword);
}

void SuperAdmin::previewAllAdmins(const Pharmacy &pharmacy) const
{
    pharmacy.previewAllAdmins();
}

// Implementation of the Admin virtual functions in the SubAdmin class
void SubAdmin::addAdmin(Pharmacy &pharmacy, const string &newAdmin, const string &newPassword)
{
    pharmacy.addAdmin(newAdmin, newPassword);
}

void SubAdmin::deleteAdmin(Pharmacy &pharmacy, const string &adminToDelete)
{
    pharmacy.deleteAdmin(adminToDelete);
}

void SubAdmin::editAdmin(Pharmacy &pharmacy, const string &adminToEdit, const string &newPassword)
{
    pharmacy.editAdmin(adminToEdit, newPassword);
}

void SubAdmin::previewAllAdmins(const Pharmacy &pharmacy) const
{
    pharmacy.previewAllAdmins();
}

int Customer::nextCustomerNumber = 1000;

int main()
{
    srand(time(0));

    Authentication auth;
    cout << "=== Welcome to the Authentication System ===\n";

    int userType;
    cout << "Select user type:\n";
    cout << "1. Administrator\n";
    cout << "2. Customer\n";

    cout << "Enter your choice: ";
    cin >> userType;
    system("cls");
    if (userType == 1)
    {
        string username, password;

        cout << "Username: ";
        cin >> username;

        cout << "Password: ";
        cin >> password;
        Pharmacy pharmacy;

        if (auth.authenticateAdmin(username, password))
        {
            system("cls");

            cout << endl
                 << endl;
            cout << "Authentication successful! Welcome, Admin." << endl;

            int adminChoice;
            do
            {
                adminMenu();
                cin >> adminChoice;
                cout << "You typed " << adminChoice << endl;
                switch (adminChoice)
                {
                case 1:
                {
                    int menuChoice;
                    clearScreen();
                    cout << "=== Admin Menu ===\n";
                    cout << "1. Add Admin\n";
                    cout << "2. Delete Admin\n";
                    cout << "3. Edit Admin\n";
                    cout << "4. Preview Admin\n";
                    cout << "5. Logout\n";
                    cout << "Enter your choice: ";
                    cin >> menuChoice;

                    switch (menuChoice)
                    {
                    case 1:
                    {
                        system("cls");
                        string newAdmin, newPassword;
                        cout << "Enter the new admin username: ";
                        cin >> newAdmin;
                        cout << "Enter the new admin password: ";
                        cin >> newPassword;
                        pharmacy.addAdmin(newAdmin, newPassword);
                        break;
                    }
                    case 2:
                    {
                        system("cls");
                        string adminToDelete;
                        cout << "Enter the username of the admin to delete: ";
                        cin >> adminToDelete;
                        pharmacy.deleteAdmin(adminToDelete);
                        break;
                    }
                    case 3:
                    {
                        system("cls");
                        string adminToEdit, newPassword;
                        cout << "Enter the username of the admin to edit: ";
                        cin >> adminToEdit;
                        cout << "Enter the new password: ";
                        cin >> newPassword;
                        pharmacy.editAdmin(adminToEdit, newPassword);
                        break;
                    }
                    case 4:
                    {
                        system("cls");
                        pharmacy.previewAllAdmins();
                        break;
                    }
                    case 5:
                    {
                        system("cls");
                        cout << "Logging out...\n";
                        return 0;
                        break;
                    }
                    default:
                    {
                        system("cls");
                        cout << "Invalid choice. Try again.\n";
                        break;
                    }
                    }
                    break; // Add this break statement
                }
                case 2:
                {
                    system("cls");
                    int MedChoice;
                    cout << "=== Medicine Menu ===\n";
                    cout << "1. Add Stock\n";
                    cout << "2. Preview All Medicines\n";
                    cout << "3. Logout\n";
                    cout << "Enter your choice: ";
                    cin >> MedChoice;

                    switch (MedChoice)
                    {
                    case 1:
                    {
                        system("cls");
                        string name;
                        int quantity;
                        double price;
                        string manufacturer;

                        cout << "Enter Medication details:" << endl;
                        cout << "Name: ";
                        cin >> name;
                        cout << "Quantity: ";
                        cin >> quantity;
                        cout << "Price: $";
                        cin >> price;
                        cout << "Manufacturer: ";
                        cin.ignore();
                        getline(cin, manufacturer);

                        Medication newMedication(name, quantity, price, manufacturer);
                        pharmacy.medicines.push_back(newMedication);

                        cout << "Medication added to Inventory." << endl;

                        ofstream outFile("medicines.txt", ios::app);
                        if (outFile.is_open())
                        {
                            outFile << name << " " << quantity << " " << price << " " << manufacturer << "\n";
                            outFile.close();
                            cout << "Medication details stored in file.\n";
                        }
                        else
                        {
                            cerr << "Error opening file for writing.\n";
                        }

                        break;
                    }
                    case 2:
                    {
                        system("cls");
                        pharmacy.displayMedicines();
                        break;
                    }
                    case 3:
                    {
                        system("cls");
                        cout << "Logging out...\n";
                        return 0;
                        break;
                    }
                    default:
                    {
                        cout << "Invalid choice. Try again.\n";
                        break;
                    }
                    }
                    break; // Add this break statement
                }
                case 3:
                {
                    system("cls");
                    cout << "Logging out...\n";
                    break;
                }
                default:
                {
                    system("cls");
                    cout << "Invalid choice. Try again.\n";
                    break;
                }
                }
            } while (adminChoice != 3);
        }
        else
        {
            system("cls");
            cout << "Authentication failed. Incorrect username or password." << endl;
        }
    }
    else if (userType == 2)
    {
        cout << "Welcome, Customer." << endl;
        string customerName;
        cout << "Enter customer's name: ";
        cin >> customerName;
        system("cls");

        srand(time(0));
        int randomValue = rand();

        cout << "Welcome, Customer " << customerName << "!\n";
        cout << "Your Order ID is: " << randomValue << endl;
        Customer customer(customerName);
        Pharmacy pharmacy;

        int customerChoice;
        do
        {
            customerMenu();
            cin >> customerChoice;

            switch (customerChoice)
            {
            case 1:
                customer.viewMedications(pharmacy);
                break;
            case 2:
                customer.searchMedications(pharmacy);
                break;
            case 3:
            {
                do
                {
                    customer.purchaseMedications(pharmacy);
                    char buyMore;
                    cout << "Do you want to buy more? (y/n): ";
                    cin >> buyMore;

                    if (buyMore != 'y' && buyMore != 'Y')
                    {
                        break;
                    }
                } while (true);
                break;
            }
            case 4:
                customer.viewOrderHistory();
                break;
            case 5:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        } while (customerChoice != 5);
    }
    else
    {
        cout << "Invalid user type." << endl;
    }

    if (userType != 1 && userType != 2)
    {
        cout << "Invalid user type. Exiting...\n";
        return 1;
    }

    return 0;
}
