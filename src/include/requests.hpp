#ifndef REQUESTS_HPP
#define REQUESTS_HPP

#include <string>
#include <vector>

/**
 * @param host host server
 * @param url URL path of the request
 * @param query_params query parameters of the request
 * @param token authentication token for the request
 * @param cookies cookies to include in the request
 * @param cookies_count number of cookies
 * @return computed GET request message as a char array
 */
char *GET(
    std::string host, std::string url, 
    std::string query_params, std::string token, 
    std::vector<std::string> cookies, int cookies_count
);

/**
 * @param host host server
 * @param url URL path of the request
 * @param token authentication token for the request
 * @param content_type content type of the request
 * @param body body data of the request
 * @param body_fields_nr number of fields in the body data
 * @param cookies cookies to include in the request
 * @param cookies_count number of cookies
 * @return computed POST request message as a char array
 */
char *POST(
    std::string host, std::string url,
    std::string token, std::string content_type, 
    std::string body, int body_fields_nr, 
    std::vector<std::string> cookies, int cookies_count
);

/**
 * @param host host server
 * @param url URL path of the request
 * @param token authentication token for the request
 * @param content_type content type of the request
 * @param body body data of the request
 * @param body_fields_nr number of fields in the body data
 * @param cookies cookies to include in the request
 * @param cookies_count number of cookies
 * @return computed DELETE request message as a char array
 */
char *DELETE(
    std::string host, std::string url, 
    std::string token, std::string content_type, 
    std::string body, int body_fields_nr, 
    std::vector<std::string> cookies, int cookies_count
);

#endif /* REQUESTS_HPP */
