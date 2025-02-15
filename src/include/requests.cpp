#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "../utils/helpers.hpp"

#include "requests.hpp"

/**
 * Constructs a GET request message.
 *
 * @param host          Server host address.
 * @param url           Target URL.
 * @param query_params  Query parameters (optional).
 * @param token         Authorization token (optional).
 * @param cookies       List of cookies (optional).
 * @param cookies_count Number of cookies.
 * @return Pointer to the constructed GET request message.
 */
char* GET(std::string host, std::string url, 
          std::string query_params, std::string token,
          std::vector<std::string> cookies, int cookies_count)
{
    char line[LINELEN];
    char* message = new char[2 * BUFFLEN];
    char* cookiesString = new char[BUFFLEN];

    // Construct the GET request line
    if (!query_params.empty())
        snprintf(line, sizeof(line), "GET %s?%s HTTP/1.1", url.c_str(), query_params.c_str());
    else
        snprintf(line, sizeof(line), "GET %s HTTP/1.1", url.c_str());

    httpMessage(message, line);

    // Add authorization token if provided
    if (!token.empty()) {
        strcat(message, "Authorization: Bearer ");
        httpMessage(message, token.c_str());
    }

    // Add host header
    strcat(message, "Host: ");
    httpMessage(message, host.c_str());

    // Add cookies if provided
    if (!cookies.empty()) {
        strcpy(cookiesString, "Cookie:");
        for (const auto& cookie : cookies) {
            strcat(cookiesString, " ");
            strcat(cookiesString, cookie.c_str());
            strcat(cookiesString, ";");
        }
        httpMessage(message, cookiesString);
    }

    // Add a blank line to separate headers from body
    httpMessage(message, "");

    delete[] cookiesString;
    return message;
}

/**
 * Constructs a POST request message.
 *
 * @param host          Server host address.
 * @param url           Target URL.
 * @param token         Authorization token (optional).
 * @param content_type  Content type of the request body.
 * @param body          Request body (JSON payload).
 * @param body_fields_nr Number of fields in the request body.
 * @param cookies       List of cookies (optional).
 * @param cookies_count Number of cookies.
 * @return Pointer to the constructed POST request message.
 */
char* POST(std::string host, std::string url, 
           std::string token, std::string content_type,
           std::string body, int body_fields_nr, 
           std::vector<std::string> cookies, int cookies_count)
{
    char* message = new char[2 * BUFFLEN];
    char* cookiesString = new char[BUFFLEN];

    // Construct the POST request line
    snprintf(message, 2 * BUFFLEN, "POST %s HTTP/1.1\r\n", url.c_str());

    // Add host header
    strcat(message, "Host: ");
    httpMessage(message, host.c_str());

    // Add authorization token if provided
    if (!token.empty()) {
        strcat(message, "Authorization: Bearer ");
        httpMessage(message, token.c_str());
    }

    // Add content type header
    strcat(message, "Content-Type: ");
    httpMessage(message, content_type.c_str());

    // Add content length header
    char content_length[32];
    snprintf(content_length, sizeof(content_length), "Content-Length: %d\r\n", body_fields_nr);
    strcat(message, content_length);

    // Add cookies if provided
    if (!cookies.empty()) {
        strcpy(cookiesString, "Cookie:");
        for (const auto& cookie : cookies) {
            strcat(cookiesString, " ");
            strcat(cookiesString, cookie.c_str());
            strcat(cookiesString, ";");
        }
        httpMessage(message, cookiesString);
    }

    // Add a blank line to separate headers from body
    httpMessage(message, "");

    // Add the request body
    httpMessage(message, body.c_str());

    delete[] cookiesString;
    return message;
}

/**
 * Constructs a DELETE request message.
 *
 * @param host          Server host address.
 * @param url           Target URL.
 * @param token         Authorization token (optional).
 * @param content_type  Content type of the request body.
 * @param body          Request body (JSON payload, optional).
 * @param body_fields_nr Number of fields in the request body.
 * @param cookies       List of cookies (optional).
 * @param cookies_count Number of cookies.
 * @return Pointer to the constructed DELETE request message.
 */
char* DELETE(std::string host, std::string url, 
             std::string token, std::string content_type,
             std::string body, int body_fields_nr,
             std::vector<std::string> cookies, int cookies_count)
{
    char* message = new char[2 * BUFFLEN];
    char* cookiesString = new char[BUFFLEN];

    // Construct the DELETE request line
    snprintf(message, 2 * BUFFLEN, "DELETE %s HTTP/1.1\r\n", url.c_str());

    // Add host header
    strcat(message, "Host: ");
    httpMessage(message, host.c_str());

    // Add authorization token if provided
    if (!token.empty()) {
        strcat(message, "Authorization: Bearer ");
        httpMessage(message, token.c_str());
    }

    // Add content type header
    strcat(message, "Content-Type: ");
    httpMessage(message, content_type.c_str());

    // Add content length header
    char content_length[32];
    snprintf(content_length, sizeof(content_length), "Content-Length: %d\r\n", body_fields_nr);
    strcat(message, content_length);

    // Add cookies if provided
    if (!cookies.empty()) {
        strcpy(cookiesString, "Cookie:");
        for (const auto& cookie : cookies) {
            strcat(cookiesString, " ");
            strcat(cookiesString, cookie.c_str());
            strcat(cookiesString, ";");
        }
        httpMessage(message, cookiesString);
    }

    // Add a blank line to separate headers from body
    httpMessage(message, "");

    // Add the request body
    httpMessage(message, body.c_str());

    delete[] cookiesString;
    return message;
}
