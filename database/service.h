#ifndef AUTHOR_H
#define AUTHOR_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Service{
        private:
            long _id;
            std::string _name;
            std::string _category;
            std::string _method;
            std::string _description;
            std::string _schedule;
            std::string _price;
            std::string _login;
            std::string _password;

        public:

            static Service fromJSON(const std::string & str);

            long             get_id() const;
            const std::string &get_name() const;
            const std::string &get_category() const;
            const std::string &get_method() const;
            const std::string &get_description() const;
            const std::string &get_schedule() const;
            const std::string &get_price() const;
            const std::string &get_login() const;
            const std::string &get_password() const;

            long&        id();
            std::string &name();
            std::string &category();
            std::string &method();
            std::string &description();
            std::string &schedule();
            std::string &price();
            std::string &login();
            std::string &password();

            static void init();
            static std::vector<Service> read_all();
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif