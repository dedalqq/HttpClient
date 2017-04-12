#include "result.h"

using namespace std;

namespace httpClient {

    Result::Result(int h_code, string h_body, Head *h_headers) {
        code = h_code;
        body = h_body;
        headers = h_headers;
    }

    string Result::getBody() {
        return body;
    }

    Head *Result::getHeader() {
        return headers;
    }

    Result::~Result() {
        delete[] headers;
    }
}
