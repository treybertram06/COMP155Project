//
// Created by treyb on 2025-03-24.
//

#ifndef LEAVEMANAGER_H
#define LEAVEMANAGER_H

#include "leave_request.h"
#include <vector>
#include <iostream>

//source for where I learned this since we haven't covered it in class
//https://www.geeksforgeeks.org/singleton-design-pattern/

class LeaveManager {
private:
    std::vector<LeaveRequest> leave_requests;
    LeaveManager() {} //private constructor to prevent instantiation

public:
    //delete copy constructor and assignment operator to prevent copies
    LeaveManager(LeaveManager&) = delete;
    LeaveManager& operator=(LeaveManager&) = delete;

    static LeaveManager& get_instance() {
        static LeaveManager instance;  //guarantee to be only initialized once
        return instance;
    }

    void add_leave_request(LeaveRequest& request) {
        leave_requests.push_back(request);
    }

    std::vector<LeaveRequest>& get_requests() { return leave_requests; }

    void print_leave_requests() {
        for (auto& request : leave_requests) {
            request.display_info();
        }
    }

};

#endif //LEAVEMANAGER_H
