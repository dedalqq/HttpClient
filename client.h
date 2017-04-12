#include <pthread.h>
#include <vector>

#include "result.h"

using namespace std;

#define INPUT_BUFER_SIZE 100

#define METHOD_GET "GET"
#define METHOD_POST "POST"

namespace httpClient {

    typedef struct url {
        string host;
        string resource;
    } Url;

    class AsyncRequestData;

    class Client {

    private:
        struct in_addr getAddrByName(string url);
        string transport(string host, string res, unsigned int port, string method);
        Url parceUrl(string url);

    public:
        Client();
        Result *request(string url, string method);
        Result *get(string url);
        AsyncRequestData *asyncGet(string url);
    };

    class AsyncRequestData {

    public:
        Result *result;
        pthread_t thread;

        string url;
        string method;
        Client *client;

        AsyncRequestData(Client *cl, string u, string m);
        ~AsyncRequestData();

        Result *getResult();
    };

    static void *asyncRequest(void *data);
}
