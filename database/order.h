#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include <optional>

namespace database
{
    class Order{
        private:
            long _id;
            long _user_id;
            long _service_id;
            //time_t _creation_date;

        public:

            static Order fromJSON(const std::string & str);

            long             get_id() const;
            long             get_user_id() const;
            long             get_service_id() const;

            long&        id();
            long&        user_id();
            long&        service_id();

            static void init();
            static std::vector<Order> read_by_user_id(long user_id);
            void save_to_mysql();

            Poco::JSON::Object::Ptr toJSON() const;

    };
}

#endif