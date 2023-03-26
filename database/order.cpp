#include "order.h"
#include "database.h"
#include "../config/config.h"

#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database
{

    void Order::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Order` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`user_id` INT NOT NULL,"
                        << "`service_id` INT NOT NULL,"
                        << "PRIMARY KEY (`id`),"
                        << "KEY `ORD_FK_User_id` (`user_id`),"
                        << "CONSTRAINT `ORD_FK_user_id` "
                        << "FOREIGN KEY (`user_id`) "
                        << "REFERENCES  `User` (`id`),"
                        << "KEY `ORD_FK_Service_id` (`service_id`),"
                        << "CONSTRAINT `ORD_FK_service_id` "
                        << "FOREIGN KEY (`service_id`) "
                        << "REFERENCES  `Service` (`id`));",
                now;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    Poco::JSON::Object::Ptr Order::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("user_id", _user_id);
        root->set("service_id", _service_id);

        return root;
    }

    // Order Order::fromJSON(const std::string &str)
    // {
    //     Order order;
    //     Poco::JSON::Parser parser;
    //     Poco::Dynamic::Var result = parser.parse(str);
    //     Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

    //     order.id() = object->getValue<long>("id");
    //     order.first_name() = object->getValue<std::string>("first_name");
    //     order.last_name() = object->getValue<std::string>("last_name");
    //     order.email() = object->getValue<std::string>("email");
    //     order.login() = object->getValue<std::string>("login");
    //     order.password() = object->getValue<std::string>("password");

    //     return order;
    // }

    std::vector<Order> Order::read_by_user_id(long user_id)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            std::vector<Order> result;
            Order a;
            select << "SELECT id, user_id, service_id FROM `Order` where user_id=?",
                into(a._id),
                into(a._user_id),
                into(a._service_id),
                use(user_id),
                range(0, 1); //  iterate over result set one row at a time

            while (!select.done())
            {
                if (select.execute())
                    result.push_back(a);
            }
            return result;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    Order Order::save_to_mysql(long user_id)
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            Order ord;

            std::cout << "order initiated \n";

            insert << "INSERT INTO `Order` (user_id,service_id) VALUES(?, ?)",
                use(user_id),
                use(_service_id);
                std::cout << "use initiated\n";

            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(ord._id),
                range(0, 1); //  iterate over result set one row at a time

            std::cout << "select initiated\n";

            if (!select.done())
            {
                select.execute();
            }

            ord._user_id = user_id;
            // ord._service_id = service_id;

            std::cout << "inserted:" << _id << std::endl;
            
            return ord;
        }
        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
            throw;
        }
    }

    long Order::get_id() const
    {
        return _id;
    }

    long &Order::id()
    {
        return _id;
    }

    long &Order::user_id()
    {
        return _user_id;
    }

    long &Order::service_id()
    {
        return _service_id;
    }
    
}