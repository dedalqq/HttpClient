#include <string>
#include <iostream>

#include "client.h"

using namespace httpClient;
using namespace std;

int main() {

    Client c;

    AsyncRequestData *a_data = c.asyncGet("ya.ru/qq");

    Result *res = a_data->getResult();

    cout << res->getBody() << endl;

    return 0;
}
