
#include <optional>
#include "catalogue_server/http_service_server.h"

int main(int argc, char*argv[]) 
{
    HTTPOtherWebServer app;
    return app.run(argc, argv);
}