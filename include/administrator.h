//
// Created by treyb on 2025-03-17.
//

#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "user.h"
#include <map>

class Administrator : public User {
public:
    Administrator(std::string username, std::string password, int ID, std::string position)
    : User(username, password, ID, position) {}

    void add_employee(Employee emp);
    void remove_employee(int empID);
    void approve_leave(int requestID);
    void view_all_leave_requests();
    void view_all_leave_balances();

    void display_info() override {
        cout << "Administrator: " << username << " | ID: " << ID << " | Position: " << position << endl;
    }

    void save_to_file(std::ofstream& file) override {
        file << "Admin " << username << " " << password << " " << ID << " " << position << std::endl;
    }

    void list_employees(vector<User*>& users) {
        cout << "Employee list:\n";
        int index = 0;
        for (auto& user : users) {
            if (dynamic_cast<Employee*>(user)) {
                cout << ++index << ": ";
                user->display_info();
            }
        }
        cout << endl;
    }

    void admin_interface(vector<User*>& users) {
        bool done = false;
        while (!done) {
            int input;
            cout << "Choose an option:\n"
                 << "1: View all employees.\n"
                 << "2: View all open leave requests.\n"
                 << "3: Remove an employee.\n"
                 << "4: Create an employee.\n"
                 << "5: Adjust employee leave balance.\n"
                 << "0: Quit.\n";
            cin >> input;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (input) {
                case 1: {
                    list_employees(users);
                }
                break;

                case 2: {
                    cout << "Select a request to update: \n";
                    int index = 0;
                    int selection;
                    string new_status;
                    auto& requests = LeaveManager::get_instance().get_requests();
                    for (auto& request : requests) {
                        cout << ++index << ": ";
                        request.display_info();

                    }
                    cout << "Select a request to update: ";
                    cin >> selection;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Enter updated status (Approved/Denied): ";
                    getline(cin, new_status);
                    requests[selection-1].update_status(new_status);
                }
                break;

                case 3: {
                    list_employees(users);
                    int emp_index;
                    cout << "Enter the index of the employee to remove: ";
                    cin >> emp_index;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (emp_index >= 0 && emp_index < users.size() && dynamic_cast<Employee*>(users[emp_index])) {
                        Employee* emp = dynamic_cast<Employee*>(users[emp_index]);
                        cout << "Removing employee: ";
                        emp->display_info();

                        delete emp;
                        users.erase(users.begin() + emp_index);
                        cout << "Employee removed successfully.\n";
                    } else {
                        cout << "Invalid index or employee not found.\n";
                    }
                }
                break;

                case 4: {
                    string new_username, new_password, new_position;
                    int new_leave_balance;
                    char is_admin;

                    cout << "Is this employee an administrator? (Y/N): ";
                    cin >> is_admin;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (is_admin == 'N') {
                        cout << "Enter employees new username (cannot contain whitespace): ";
                        getline(cin, new_username);
                        for (auto& user : users) {
                            if (user->get_username() == new_username) {
                                cout << "Username is already taken.\n";
                                break;
                            }
                        }
                        cout << "Enter employees new password: ";
                        getline(cin, new_password);
                        cout << "Enter employees new position: ";
                        getline(cin, new_position);
                        cout << "Enter employees leave balance: ";
                        cin >> new_leave_balance;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        users.push_back(new Employee(new_username, new_password, generate_ID(), new_position, new_leave_balance));
                    } else if (is_admin == 'Y') {
                        cout << "Enter administrator new username: ";
                        getline(cin, new_username);
                        cout << "Enter administrator new password: ";
                        getline(cin, new_password);
                        cout << "Enter administrator new position: ";
                        getline(cin, new_position);

                        users.push_back(new Administrator(new_username, new_password, generate_ID(), new_position));
                    } else {
                        cout << "Invalid input.\n";
                    }


                }
                break;

                case 5: {
                    list_employees(users);
                    int emp_index, new_balance;
                    cout << "Enter the index of the employee to adjust leave balance: ";
                    cin >> emp_index;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (emp_index >= 0 && emp_index < users.size() && dynamic_cast<Employee*>(users[emp_index])) {
                        Employee* emp = dynamic_cast<Employee*>(users[emp_index]);
                        cout << "Selected employee: ";
                        emp->display_info();

                        cout << "Enter new leave balance amount: ";
                        cin >> new_balance;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        emp->set_leave_balance(new_balance);
                        cout << "Leave balance set to: " << emp->get_leave_balance() << endl;;
                    } else {
                        cout << "Invalid index or employee not found.\n";
                    }
                }
                break;

                case 0:
                    cout << "Quitting...\n";
                done = true;
                break;

                default:
                    cout << "Invalid input.\n";
                break;
            }


        }
    }


private:
    std::map<int, Employee> employee_records;
};

#endif //ADMINISTRATOR_H
