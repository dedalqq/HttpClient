#include <string>

using namespace std;

namespace httpClient {

    class Head {
        string name;
        string value;
    };

    class Result {

    private:
        string body;
        int code;
        Head *headers;

    public:
        Result(int h_code, string h_body, Head *h_headers);
        string getBody();
        Head *getHeader();
        ~Result();

    };
}
