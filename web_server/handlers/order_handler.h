#ifndef ORDERHANDLER_H
#define ORDERHANDLER_H

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>
#include <iostream>
#include <fstream>


using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::NameValueCollection;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;

#include "../../database/order.h"
#include "../../database/service.h"
#include "../../database/user.h"
#include "../../helper.h"

class OrderHandler : public HTTPRequestHandler
{
public:
    OrderHandler(const std::string &format) : _format(format)
    {
    }

    void handleRequest(HTTPServerRequest &request,
                       HTTPServerResponse &response)
    {
        HTMLForm form(request, request.stream());

        long cur_user_id = TryAuth(request, response);

        if(cur_user_id == 0){
            //No Auth
            return;
        }
            

            if (hasSubstr(request.getURI(), "/searchOrder") && (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET))
            {
                long user_id = cur_user_id;
                try
                {
                    Poco::JSON::Array arr;
                    auto results = database::Order::read_by_user_id(user_id);
                    for (auto s : results)
                        arr.add(s.toJSON());
                    //response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();

                    Poco::JSON::Stringifier::stringify(arr, ostr);
                    return;
                }
                catch (...)
                {
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                    std::ostream &ostr = response.send();
                    ostr << "{ \"result\": false , \"reason\": \"not found\" }";
                    response.send();
                }
            }

            else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST)
            {
                long user_id = cur_user_id;
                
                if (form.has("service_id"))
                {
                    long service_id = atol(form.get("service_id").c_str());
                    database::Order order;
                    order.service_id() = service_id;
                    //std::string message;
                    //std::string reason;

                    try
                    {
                        order.save_to_mysql(user_id);
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                        response.setChunkedTransferEncoding(true);
                        response.setContentType("application/json");
                        std::ostream &ostr = response.send();
                        ostr << order.get_id();
                        return;
                    }
                    catch (...)
                    {
                        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
                        std::ostream &ostr = response.send();
                        ostr << "{ \"result\": false , \"reason\": \"exception\" }";
                        response.send();
                    }
                }
        }
    }

private:
    std::string _format;
};
#endif