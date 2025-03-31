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

    void set_leave_balance(int updated) { leave_balance = updated; }

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

    void employee_interface() {
        bool done = false;
        while (!done) {

            int input;
            cout << "Choose an option:\n"
                 << "1: Apply for leave.\n"
                 << "2: Check status of leave requests.\n"
                 << "3: Check leave balance.\n"
                 << "0: Quit.\n";
            cin >> input;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (input) {
                case 1: {
                    string leave_type, start_date;
                    int days;
                    cout << "Enter leave type: ";
                    getline(cin, leave_type);
                    cout << "Enter start date (MM/DD/YYYY): ";
                    getline(cin, start_date);
                    cout << "Enter number of days: ";
                    cin >> days;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    apply_for_leave(start_date, days, leave_type);
                }
                break;

                case 2: {
                    //cout << "Select an open leave request:\n";
                    auto requests = LeaveManager::get_instance().get_requests();
                    for (auto& request : requests) {
                        if (ID == request.get_employee_ID()) {
                            request.display_info();
                        }
                    }
                }
                break;

                case 3:
                    cout << "You have " << get_leave_balance() << " days of leave remaining.\n";
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
    int leave_balance;
};

#endif //EMPLOYEE_H
