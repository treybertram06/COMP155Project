/*
Project Description:
Create a simple console-based Leave Management System to handle employee leave requests in an organization. The system
will allow different users (employees and administrators) to perform various tasks related to leave management.

Features to Implement:
•	For Employees: Register as a new employee, apply for leave, check the status of leave request, view leave balance.
•	For Administrators: Add or remove employee records, approve or reject leave requests, view all leave requests and employee leave balances.

Principles to Use:
•	Abstraction: Use abstract classes to define common functions shared between employees and administrators, such as applying for or processing leave requests.
•	Encapsulation: Protect sensitive data by encapsulating it within classes and providing methods to access or modify this data.
•	Polymorphism: Implement method overriding where needed, allowing employees to apply for leave and admins to approve or reject leave requests.
•	Inheritance: Create a base class for common attributes (e.g., name, ID, position) that both employees and administrators can inherit from.
 */
#include <iostream>
#include <vector>

#include "../include/user.h"
#include "../include/employee.h"
#include "../include/administrator.h"
#include <iostream>
#include <vector>

using namespace std;

void save_data_to_file(vector<User*>& users, LeaveManager& leave_manager) {
    ofstream data_file("data.txt");
    for (auto& user : users) {
        user->save_to_file(data_file);
    }

    vector<LeaveRequest> leave_requests = leave_manager.get_requests();

    for (auto& req : leave_requests) {
        req.save_to_file(data_file);
    }
    data_file.close();
}

vector<User*> load_data_from_file() {
    vector<User*> users;
    ifstream file("data.txt");

    string line;
    while (getline(file, line)) {
        istringstream stream(line);

        string type;
        stream >> type;

        if (type == "Employee") {
            string username, password, position;
            int ID, leave_balance;
            stream >> username >> password >> ID >> position >> leave_balance;
            users.push_back(new Employee(username, password, ID, position, leave_balance));
        }
        else if (type == "Admin") {
            string username, password, position;
            int ID;
            stream >> username >> password >> ID >> position;
            users.push_back(new Administrator(username, password, ID, position));
        }
        else if (type == "LeaveRequest") {
            int request_ID, user_ID, days, start_date;
            string leave_type, status;
            stream >> request_ID >> user_ID >> status >> leave_type >> start_date >> days;
            LeaveRequest request(request_ID, user_ID, status, leave_type, start_date, days);
            LeaveManager::get_instance().add_leave_request(request);
        }
    }

    file.close();
    return users;
}

User* try_login(vector<User*> users, string username, string pass) {
    for (const auto& user : users) {
        if (user->check_login_info(username, pass))
            return user;
    }
    return nullptr;
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

void admin_interface(User* user, vector<User*>& users) {
    bool done = false;
    while (!done) {
        int input;
        cout << "Choose an option:\n"
             << "1: View all employees.\n"
             << "2: View all open leave requests.\n"
             << "3: Remove an employee.\n"
             << "4: Create an employee.\n"
             << "0: Quit.\n";
        cin >> input;

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
                cout << "Enter updated status (Approved/Denied): ";
                cin >> new_status;
                requests[selection-1].update_status(new_status);
            }
            break;

            case 3: {
                list_employees(users);
                int emp_index;
                cout << "Enter the index of the employee to remove: ";
                cin >> emp_index;

                // Validate index range
                if (emp_index >= 0 && emp_index < users.size() && dynamic_cast<Employee*>(users[emp_index])) {
                    Employee* emp = dynamic_cast<Employee*>(users[emp_index]);
                    cout << "Removing employee: ";
                    emp->display_info();  // Show info of the employee to be removed

                    // Properly delete and remove employee
                    delete emp;
                    users.erase(users.begin() + emp_index);  // Safely remove from vector
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
                if (is_admin == 'N') {
                    cout << "Enter employees new username (cannot contain whitespace): ";
                    cin >> new_username;
                    for (auto& user : users) {
                        if (user->get_username() == new_username) {
                            cout << "Username is already taken.\n";
                            break;
                        }
                    }
                    cout << "Enter employees new password: ";
                    cin >> new_password;
                    cout << "Enter employees new position: ";
                    cin >> new_position;
                    cout << "Enter employees leave balance: ";
                    cin >> new_leave_balance;

                    users.push_back(new Employee(new_username, new_password, generate_ID(), new_position, new_leave_balance));
                } else if (is_admin == 'Y') {
                    cout << "Enter administrator new username: ";
                    cin >> new_username;
                    cout << "Enter administrator new password: ";
                    cin >> new_password;
                    cout << "Enter administrator new position: ";
                    cin >> new_position;

                    users.push_back(new Administrator(new_username, new_password, generate_ID(), new_position));
                } else {
                    cout << "Invalid input.\n";
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

void employee_interface(User* user_ptr) {
    Employee* user = dynamic_cast<Employee*>(user_ptr);
    bool done = false;
    while (!done) {

        int input;
        cout << "Choose an option:\n"
             << "1: Apply for leave.\n"
             << "2: Check status of leave requests.\n"
             << "3: Check leave balance.\n"
             << "0: Quit.\n";
        cin >> input;

        switch (input) {
            case 1: {
                string leave_type, start_date;
                int days;
                cout << "Enter leave type: ";
                cin >> leave_type;
                cout << "Enter start date (MM/DD/YYYY): ";
                cin >> start_date;
                cout << "Enter number of days: ";
                cin >> days;
                user->apply_for_leave(start_date, days, leave_type);
            }
            break;

            case 2: {
                //cout << "Select an open leave request:\n";
                auto requests = LeaveManager::get_instance().get_requests();
                for (auto& request : requests) {
                    if (user_ptr->get_ID() == request.get_employee_ID()) {
                        request.display_info();
                    }
                }
            }

            break;

            case 3:
                cout << "You have " << user->get_leave_balance() << " days of leave remaining.\n";
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



int main() {

    //get users from file
    bool contains_admin = false;
    vector<User*> users = load_data_from_file();
    for (auto& user : users) {
        if (user->get_username() == "admin") {
            contains_admin = true;
            //break;
        }
    }
    if (!contains_admin) {
        users.push_back(new Administrator("admin", "admin", 0, "administrator"));
    }



    // Prompt for login information, try login, if valid: current_user = (ptr to user with matching login info)
    bool logged_in = false;
    User* current_user = nullptr;
    while (!logged_in) {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        current_user = try_login(users, username, password);
        if (current_user)
            logged_in = true;
        else
            cout << "Username or password are incorrect.\n";
    }
    //clog << "Logged in successfully.\n";

    //check which type of user is logged in and start appropriate interface
    //dynamic cast will return 0 if the type of current_user is not Administrator*
    if (dynamic_cast<Administrator*>(current_user)) {
        //clog << "Is admin\n";
        admin_interface(current_user, users);
    } else {
        //clog << "Is employee\n";
        employee_interface(current_user);
    }

    save_data_to_file(users, LeaveManager::get_instance());

    for (auto user : users) {
        delete user;
    }

    return 0;
}