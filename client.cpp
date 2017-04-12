#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <signal.h>

#include <iostream>
#include <string>

#include "client.h"

namespace httpClient {

    AsyncRequestData::AsyncRequestData(Client *cl, string u, string m) {
        url = u;
        client = cl;
        method = m;
    }

    AsyncRequestData::~AsyncRequestData() {
        if (result != NULL) {
            delete result;
        }
    }

    Result *AsyncRequestData::getResult() {
        pthread_join(thread, NULL);
        return result;
    }

    Client::Client() {

    }

    struct in_addr Client::getAddrByName(string url) {

    }

    Url Client::parceUrl(string url) {
        Url result;
        for (int i = 0; i < url.size(); i++) {
            if (url[i] == '/') {
                result.host = url.substr(0, i);
                result.resource = url.substr(i, url.size());
                break;
            }
        }
        return result;
    }

    string Client::transport(string host, string res, unsigned int port, string method) {

        int sock;
        struct sockaddr_in server_info;
        string input_str;

        sock = socket(AF_INET , SOCK_STREAM , 0);

        if (sock == -1) {
            cerr << "Could not create socket" << '\n';
            return input_str;
        }

        struct hostent *server = gethostbyname(host.c_str());
        if (NULL == server) {
            cerr << "dns error;" << endl;
            return input_str;
        }

        server_info.sin_addr = *(struct in_addr *)server->h_addr;
        server_info.sin_port = htons(port);
        server_info.sin_family = AF_INET;

        if (0 != connect(sock, (struct sockaddr *)&server_info, sizeof(server_info))) {
            cerr << "Could not connect" << endl;
            return input_str;
        }

        int request_size = host.size() + res.size() + method.size() + 19;
        char *request_header = new char[request_size];
        sprintf(request_header, "%s %s HTTP/1.1\nhost: %s\n\n", method.c_str(), res.c_str(), host.c_str());

        int send_len = send(sock , request_header , request_size , 0);

        delete[] request_header;

        if (send_len != request_size) {
            cerr << "Socket error" << endl;
            return input_str;
        }

        int recv_len;
        char data_bufer[INPUT_BUFER_SIZE + 1];
        data_bufer[INPUT_BUFER_SIZE] = '\0';

        while (true) {
            recv_len = recv(sock , data_bufer , INPUT_BUFER_SIZE , 0);
            if (recv_len == INPUT_BUFER_SIZE) {
                input_str += string(data_bufer);
            }
            else if (0 == recv_len) {
                break;
            }
            else {
                input_str += string(data_bufer).substr(0, recv_len);
            }
        }

        return input_str;
    }

    Result *Client::request(string url, string method) {

        Url url_struct = parceUrl(url);

        string result_content = transport(url_struct.host, url_struct.resource, 80, method);

        string header;
        string body;

        for (int i = 0; i < result_content.size() - 1; i++) {
            if (result_content[i] == '\n' && result_content[i+2] == '\n') {
                header = result_content.substr(0, i-1);
                body = result_content.substr(i+3, result_content.size() - 1);
            }
        }

        Head *headers = new Head[5];
        // TODO парсим и заполняем заголовки
        Result *result = new Result(0, body, headers);

        return result;
    };

    Result *Client::get(string url) {
        return request(url, METHOD_GET);
    }

    AsyncRequestData *Client::asyncGet(string url) {

        AsyncRequestData *request_data = new AsyncRequestData(this, url, METHOD_GET);
        int result = pthread_create(&request_data->thread, NULL, asyncRequest, request_data);
        if (0 != result) {
            cerr << "Thread error" << endl;
        }

        return request_data;
    }

    static void *asyncRequest(void *data) {
        AsyncRequestData *request_data = (AsyncRequestData *)data;
        request_data->result = request_data->client->request(request_data->url, request_data->method);
        pthread_exit(&request_data->thread);
    }
}
