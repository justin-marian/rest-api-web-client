#ifndef LOGIN_HPP
#define LOGIN_HPP

#include "../response.hpp"

/**
 * Handles user login by sending credentials to the server.
 *
 * @param conn   Connection string for the server.
 * @param sockfd Socket file descriptor for communication.
 * @param loginB Boolean flag indicating if the user is already logged in.
 * @param reply  Reference to a string where the server response code will be stored.
 * @param cookie Reference to a string where the session cookie will be stored upon successful login.
 */
void login(char *conn, int &sockfd, bool &loginB, std::string &reply, std::string &cookie)
{
    // Check if the user is already logged in
    if (loginB) {
        std::cout << "INFO: You are already logged in!" << std::endl;
        return;
    }

    std::string username;
    std::string password;
    nlohmann::json json;

    // Get username from the user
    std::cout << "Enter username: ";
    std::getline(std::cin >> std::ws, username);
    json["username"] = username;

    // Get password from the user
    std::cout << "Enter password: ";
    std::getline(std::cin >> std::ws, password);
    json["password"] = password;

    // Convert JSON object to a string payload
    std::string jsonPayload = json.dump();
    size_t payloadLength = jsonPayload.length();

    // Construct and send the POST request for login
    // - conn: The server connection details.
    // - LOGIN: The API endpoint for user login.
    // - NO_TOKEN: No additional authorization token needed.
    // - APP: The content type (usually "application/json").
    // - jsonPayload: The serialized JSON payload containing login credentials.
    // - payloadLength: The length of the JSON string.
    // - {}: No additional headers.
    // - 0: No extra parameters.
    std::string message = POST(conn, LOGIN, NO_TOKEN, APP, jsonPayload, payloadLength, {}, 0);
    sendServerMessage(sockfd, message);

    // Receive the server's response
    std::string response;
    extractServerResponse(response, sockfd);

    // Extract response code and JSON content (if any)
    reply = extractJSONCode(response);
    std::string jsonResponse = extractJSONResponse(response);

    // Check if the response is empty (unexpected error)
    if (jsonResponse.empty()) {
        std::cout << "ERROR: Unknown problem occurred during login!" << std::endl;
        return;
    }

    // Parse the JSON response
    nlohmann::json responseJSON;
    try {
        responseJSON = nlohmann::json::parse(jsonResponse);
    } catch (const std::exception &e) {
        std::cout << "ERROR: Failed to parse server response!" << std::endl;
        return;
    }

    // Extract session cookie if login is successful
    std::string sidIndicator = "connect.sid=";
    size_t sidPos = response.find(sidIndicator);
    if (sidPos != std::string::npos) {
        std::string cookieStr = response.substr(sidPos);
        cookie = cookieStr.substr(0, cookieStr.find(";")); // Extract session cookie
    } else {
        std::cout << "ERROR: Session cookie not found in response!" << std::endl;
        return;
    }

    // Handle the JSON error response from the server
    errorJSONReply(jsonResponse, reply);

    // Successfully logged in
    loginB = true;
    std::cout << "SUCCESS: " << reply << " - Logged in successfully." << std::endl;
}

#endif /* LOGIN_HPP */
