#ifndef SERVICE_H
#define SERVICE_H

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
            long _user_id;

        public:

            static Service fromJSON(const std::string & str);

            long             get_id() const;
            const std::string &get_name() const;
            const std::string &get_category() const;
            const std::string &get_method() const;
            const std::string &get_description() const;
            const std::string &get_schedule() const;
            const std::string &get_price() const;

            long&        id();
            std::string &name();
            std::string &category();
            std::string &method();
            std::string &description();
            std::string &schedule();
            std::string &price();
            long&        user_id();

            static void init();
            static std::vector<Service> read_all();
            Service save_to_mysql(long user_id);

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif