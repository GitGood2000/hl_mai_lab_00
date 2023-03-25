#include "service.h"
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

    void Service::init()
    {
        try
        {

            Poco::Data::Session session = database::Database::get().create_session();
            Statement create_stmt(session);
            create_stmt << "CREATE TABLE IF NOT EXISTS `Service` (`id` INT NOT NULL AUTO_INCREMENT,"
                        << "`name` VARCHAR(256) NOT NULL,"
                        << "`category` VARCHAR(256) NOT NULL,"
                        << "`method` VARCHAR(256) NOT NULL,"
                        << "`description` VARCHAR(256) NOT NULL,"
                        << "`schedule` VARCHAR(256) NOT NULL,"
                        << "`price` VARCHAR(256) NOT NULL,"
                        << "`login` VARCHAR(256) NOT NULL,"
                        << "`password` VARCHAR(256) NOT NULL,"
                        << "PRIMARY KEY (`id`),KEY `categ` (`category`),KEY `login` (`login`));",
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

    Poco::JSON::Object::Ptr Service::toJSON() const
    {
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

        root->set("id", _id);
        root->set("name", _name);
        root->set("category", _category);
        root->set("method", _method);
        root->set("description", _description);
        root->set("schedule", _schedule);
        root->set("price", _price);
        root->set("login", _login);
        root->set("password", _password);

        return root;
    }

    Service Service::fromJSON(const std::string &str)
    {
        Service service;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

        service.id() = object->getValue<long>("id");
        service.name() = object->getValue<std::string>("name");
        service.category() = object->getValue<std::string>("category");
        service.method() = object->getValue<std::string>("method");
        service.description() = object->getValue<std::string>("description");
        service.schedule() = object->getValue<std::string>("schedule");
        service.price() = object->getValue<std::string>("price");
        service.login() = object->getValue<std::string>("login");
        service.password() = object->getValue<std::string>("password");

        return service;
    }

    std::optional<long> Service::auth(std::string &login, std::string &password)
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement select(session);
            long id;
            select << "SELECT id FROM Service where login=? and password=?",
                into(id),
                use(login),
                use(password),
                range(0, 1); //  iterate over result set one row at a time

            select.execute();
            Poco::Data::RecordSet rs(select);
            if (rs.moveFirst()) return id;
        }

        catch (Poco::Data::MySQL::ConnectionException &e)
        {
            std::cout << "connection:" << e.what() << std::endl;
        }
        catch (Poco::Data::MySQL::StatementException &e)
        {

            std::cout << "statement:" << e.what() << std::endl;
        }
        return {};
    }

    std::vector<Service> Service::read_all()
    {
        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Statement select(session);
            std::vector<Service> result;
            Service a;
            select << "SELECT id, name, category, method, description, schedule, price, login, password FROM Service",
                into(a._id),
                into(a._name),
                into(a._category),
                into(a._method),
                into(a._description),
                into(a._schedule),
                into(a._price),
                into(a._login),
                into(a._password),
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

    void Service::save_to_mysql()
    {

        try
        {
            Poco::Data::Session session = database::Database::get().create_session();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Service (id, name, category, method, description, schedule, price, login, password) VALUES(?, ?, ?, ?, ?, ?, ?)",
                use(_name),
                use(_category),
                use(_method),
                use(_description),
                use(_schedule),
                use(_price),
                use(_login),
                use(_password);

            insert.execute();

            Poco::Data::Statement select(session);
            select << "SELECT LAST_INSERT_ID()",
                into(_id),
                range(0, 1); //  iterate over result set one row at a time

            if (!select.done())
            {
                select.execute();
            }
            std::cout << "inserted:" << _id << std::endl;
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

    const std::string &Service::get_login() const
    {
        return _login;
    }

    const std::string &Service::get_password() const
    {
        return _password;
    }

    std::string &Service::login()
    {
        return _login;
    }

    std::string &Service::password()
    {
        return _password;
    }

    long Service::get_id() const
    {
        return _id;
    }

    const std::string &Service::get_name() const
    {
        return _name;
    }

    const std::string &Service::get_category() const
    {
        return _category;
    }

    const std::string &Service::get_method() const
    {
        return _method;
    }

    const std::string &Service::get_description() const
    {
        return _description;
    }

    const std::string &Service::get_schedule() const
    {
        return _schedule;
    }

    const std::string &Service::get_price() const
    {
        return _price;
    }

    long &Service::id()
    {
        return _id;
    }

    std::string &Service::name()
    {
        return _name;
    }

    std::string &Service::category()
    {
        return _category;
    }

    std::string &Service::method()
    {
        return _method;
    }

    std::string &Service::description()
    {
        return _description;
    }

    std::string &Service::schedule()
    {
        return _schedule;
    }

    std::string &Service::price()
    {
        return _price;
    }
}