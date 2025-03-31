//
// Created by treyb on 2025-03-17.
//

#ifndef USER_H
#define USER_H
#include <string>

class User {
public:
    User(std::string username, std::string password, int ID, std::string position) : username(username), password(password), position(position), ID(ID) {}
    virtual ~User() {}

    virtual void display_info() = 0;

    bool check_login_info(std::string tried_user, std::string tried_pass) {
        return (tried_user == username && tried_pass == password);
    }

    int get_ID() { return ID; }

    virtual void save_to_file(std::ofstream& file) = 0;

    std::string get_username() { return username; }

protected:
    std::string username, password, position;
    int ID;

};

#endif //USER_H
