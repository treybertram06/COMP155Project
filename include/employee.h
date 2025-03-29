//
// Created by treyb on 2025-03-17.
//

#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "user.h"
#include "leave_request.h"
#include "helper.h"
#include "leave_manager.h"
#include <fstream>


class Employee : public User {
public:
    Employee(std::string username, std::string password, int ID, std::string position, int leave_balance)
    : User(username, password, ID, position), leave_balance(leave_balance) {}

    void apply_for_leave(string start_date, int days, string leave_type) {
        int start = date_to_int(start_date); 
        auto request = LeaveRequest(generate_ID(), ID, leave_type, start, days);
        if (days < leave_balance) {
            LeaveManager::get_instance().add_leave_request(request);
            leave_balance = leave_balance - days;
        } else {
            cout << "Insuffient leave days remaining\n";
        }

    }

    void check_leave_status(int request_ID);

    int get_leave_balance() { return leave_balance; }

    void display_info() override {
        cout << "Employee: " << username << " | ID: " << ID << " | Position: " << position << " | Leave Balance: " << leave_balance << endl;
    }

    void save_to_file(std::ofstream& file) override {
        file << "Employee " << username << " " << password << " " << ID << " " << position << " " << leave_balance << std::endl;
    }

    /*
    static Employee* load_from_file(std::ifstream& file) {
        std::string username, password, position;
        int ID, leave_balance;
        char comma;

        file >> username >> comma >> password >> comma >> ID >> comma >> position >> comma >> leave_balance;

        return new Employee(username, password, ID, position, leave_balance);
    }
    */

    string get_username() { return username; }



private:
    int leave_balance;
};

#endif //EMPLOYEE_H
