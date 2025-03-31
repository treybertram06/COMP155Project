//
// Created by treyb on 2025-03-17.
//

#ifndef LEAVE_REQUEST_H
#define LEAVE_REQUEST_H

#include <chrono>
#include <ratio>
#include <string>
#include <fstream>
#include "helper.h"

using namespace std;

class LeaveRequest {
private:
    int request_ID;
    int employee_ID;
    string status;
    string leave_type;
    int start_date;
    int days;

public:

    LeaveRequest(int request_ID, int employee_ID, string leave_type, int start_date, int days)
    : request_ID(request_ID), employee_ID(employee_ID), leave_type(leave_type), start_date(start_date), days(days) {
        status = "Pending";
    }

    LeaveRequest(int request_ID, int employee_ID, string status, string leave_type, int start_date, int days)
    : request_ID(request_ID), employee_ID(employee_ID), status(status), leave_type(leave_type), start_date(start_date), days(days) {}


    string get_status() { return status; }

    void update_status(string new_status) { status = new_status; }

    string get_date();

    void display_info() {
        std::cout << "Employee: " << employee_ID
                      << "\nStart Date: " << int_to_date(start_date)
                      << "\nEnd Date: " << int_to_date(days)
                      << "\nReason: " << leave_type
                      << "\nStatus: " << status
                      << "\n---------------------\n";
    }

    int get_employee_ID() { return employee_ID; }

    void save_to_file(std::ofstream& file) {
        file << "LeaveRequest " << request_ID << " " << employee_ID << " " << status << " " << leave_type << " " << start_date << " " << days << std::endl;
    }

    static LeaveRequest* load_from_file(std::ifstream& file) {
        int request_ID, employee_ID, start_date, days;
        std::string leave_type, status;
        char comma;

        file >> request_ID >> comma >> employee_ID >> comma >> status >> comma >> leave_type >> comma >> start_date >> comma >> days;

        return new LeaveRequest(request_ID, employee_ID, leave_type, start_date, days);
    }
};



#endif //LEAVE_REQUEST_H
