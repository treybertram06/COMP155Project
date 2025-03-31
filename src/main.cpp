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


using namespace std;

void save_data_to_file(vector<User*>& users, LeaveManager& leave_manager) {
    ofstream data_file("data.txt");

    data_file << "Index " << current_id << endl;

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
        else if (type == "#") {
            //is a comment, do nothing
        }
        else if (type == "Index") {
            stream >> current_id;
            //clog << current_id;
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
        getline(cin, username);
        cout << "Enter password: ";
        getline(cin, password);

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
        Administrator user = *dynamic_cast<Administrator*>(current_user);
        user.admin_interface(users);
    } else {
        //clog << "Is employee\n";
        Employee user = *dynamic_cast<Employee*>(current_user);
        user.employee_interface();
    }

    save_data_to_file(users, LeaveManager::get_instance());

    for (auto user : users) {
        delete user;
    }

    return 0;
}