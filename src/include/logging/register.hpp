#ifndef REGISTER_HPP
#define REGISTER_HPP

#include "../response.hpp"

/**
 * Handles user registration by sending credentials to the server.
 *
 * @param conn   Connection string for the server.
 * @param sockfd Socket file descriptor for communication.
 * @param login  Boolean flag indicating if the user is already logged in.
 * @param reply  Reference to a string where the server response code will be stored.
 */
void register_credentials(char *conn, int &sockfd, bool &login, std::string &reply)
{
    // Check if the user is already logged in
    if (login) {
        std::cout << "INFO: You are already logged in!" << std::endl;
        return;
    }

    nlohmann::json json;

    // Get username from the user
    std::string username;
    std::cout << "Enter username: ";
    std::getline(std::cin >> std::ws, username);
    json["username"] = username;

    // Get password from the user
    std::string password;
    std::cout << "Enter password: ";
    std::getline(std::cin >> std::ws, password);
    json["password"] = password;

    // Convert JSON object to a string payload
    std::string jsonPayload = json.dump();
    size_t payloadLength = jsonPayload.length();

    // Construct and send the POST request for registration
    // - conn: The server connection details.
    // - REGISTER: The API endpoint for user registration.
    // - NO_TOKEN: No additional authorization token needed.
    // - APP: The content type (usually "application/json").
    // - jsonPayload: The serialized JSON payload containing login credentials.
    // - payloadLength: The length of the JSON string.
    // - {}: No additional headers.
    // - 0: No extra parameters.
    std::string message = POST(conn, REGISTER, NO_TOKEN, APP, jsonPayload, payloadLength, {}, 0);
    sendServerMessage(sockfd, message);

    // Receive the server's response
    std::string response;
    extractServerResponse(response, sockfd);

    // Extract response code and JSON content (if any)
    reply = extractJSONCode(response);
    std::string jsonResponse = extractJSONResponse(response);

    // Check if the response is empty (registration successful)
    if (jsonResponse.empty()) {
        std::cout << "SUCCESS: " << reply << " - User registered successfully." << std::endl;
        return;
    }

    // Handle the JSON error response from the server
    errorJSONReply(jsonResponse, reply);
}

#endif /* REGISTER_HPP */
