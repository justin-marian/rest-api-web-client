#ifndef ENTER_HPP
#define ENTER_HPP

#include "../response.hpp"

/**
 * Attempts to enter the library if the user is logged in.
 *
 * @param conn   Connection string for the server.
 * @param sockfd Socket file descriptor for communication.
 * @param login  Boolean flag indicating if the user is logged in.
 * @param enter  Boolean flag indicating if the user has already entered the library.
 * @param jwt    Reference to a string where the JWT token will be stored upon successful entry.
 * @param reply  Reference to a string where the server response code will be stored.
 * @param cookie Session cookie required for authentication.
 */
void enter_library(char *conn, int &sockfd, bool &login, bool &enter, 
                   std::string &jwt, std::string &reply, std::string &cookie)
{
    // Check if the user is logged in
    if (!login) {
        std::cout << "ERROR: You are not logged in!" << std::endl;
        return;
    }

    // Check if the user is already inside the library
    if (enter) {
        std::cout << "INFO: You are already inside the library!" << std::endl;
        return;
    }

    // Mark the user as inside the library
    enter = true;

    // Construct and send the GET request to access the library
    // - conn: The server connection details.
    // - ACCESS: The API endpoint for entering the library.
    // - NO_QUERY: No additional query parameters needed.
    // - NO_TOKEN: No additional authorization token required.
    // - {cookie}: The session cookie required for authentication.
    // - 1: Number of additional headers (cookie in this case).
    std::string message = GET(conn, ACCESS, NO_QUERRY, NO_TOKEN, {cookie}, 1);
    sendServerMessage(sockfd, message);

    // Receive the server's response
    std::string response;
    extractServerResponse(response, sockfd);

    // Extract response code and JSON content (if any)
    reply = extractJSONCode(response);
    std::string jsonResponse = extractJSONResponse(response);

    // Check if the response is empty (unexpected error)
    if (jsonResponse.empty()) {
        std::cout << "ERROR: Unknown problem occurred!" << std::endl;
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

    // Check if the JSON response contains an error
    if (responseJSON.contains("error")) {
        std::cout << "ERROR: " << reply << " <=> " << responseJSON["error"].get<std::string>() << std::endl;
        return;
    }

    // Successfully entered the library, retrieve JWT token
    if (responseJSON.contains("token")) {
        jwt = responseJSON["token"].get<std::string>();
        std::cout << "SUCCESS: " << reply << " - Entered the library successfully." << std::endl;
    } else {
        std::cout << "ERROR: Server response did not include a valid token!" << std::endl;
    }

    // Handle the JSON error response from the server
    errorJSONReply(jsonResponse, reply);
}

#endif /* ENTER_HPP */
