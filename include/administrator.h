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

    string get_username() {return username; }

private:
    std::map<int, Employee> employee_records;
};

#endif //ADMINISTRATOR_H
