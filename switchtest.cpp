/*
                    case 1:
                    {
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
                        pharmacy.displayMedicines();
                        break;
                    }
                    case 3:
                    {
                        string newAdmin, newPassword;
                        cout << "Enter the new admin username: ";
                        cin >> newAdmin;
                        cout << "Enter the new admin password: ";
                        cin >> newPassword;
                        pharmacy.addAdmin(newAdmin, newPassword);
                        break;
                    }
                    case 4:
                    {
                        string adminToDelete;
                        cout << "Enter the username of the admin to delete: ";
                        cin >> adminToDelete;
                        pharmacy.deleteAdmin(adminToDelete);
                        break;
                    }
                    case 5:
                    {
                        string adminToEdit, newPassword;
                        cout << "Enter the username of the admin to edit: ";
                        cin >> adminToEdit;
                        cout << "Enter the new password: ";
                        cin >> newPassword;
                        pharmacy.editAdmin(adminToEdit, newPassword);
                        break;
                    }
                    case 6:
                    {
                        pharmacy.previewAllAdmins();
                        break;
                    }
                    case 7:
                    {
                        system("cls");
                        cout << "Logging out...\n";
                        break;
                    }
                    default:
                        cout << "Invalid choice. Try again.\n";
                    }*/