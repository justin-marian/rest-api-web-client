#ifndef LOGOUT_HPP
#define LOGOUT_HPP

#include "../response.hpp"

/**
 * Logs the user out of the library system.
 *
 * @param conn   Connection string for the server.
 * @param sockfd Socket file descriptor for communication.
 * @param login  Boolean flag indicating if the user is logged in.
 * @param enter  Boolean flag indicating if the user has entered the library.
 * @param jwt    Reference to a string where the JWT token will be cleared upon logout.
 * @param reply  Reference to a string where the server response code will be stored.
 * @param cookie Reference to a string where the session cookie will be cleared upon logout.
 */
void logout(char *conn, int &sockfd, bool &login, bool &enter, 
            std::string &jwt, std::string &reply, std::string &cookie)
{
    // Check if the user is logged in
    if (!login) {
        std::cout << "INFO: You are not logged in!" << std::endl;
        return;
    }

    // Construct and send the GET request for logout
    // - conn: The server connection details.
    // - LOGOUT: The API endpoint for logging out.
    // - NO_QUERY: No additional query parameters needed.
    // - NO_TOKEN: No additional authorization token required.
    // - {cookie}: The session cookie required for authentication.
    // - cookie.empty() ? 0 : 1: Determines if a cookie should be sent (avoids unnecessary headers).
    std::string message = GET(conn, LOGOUT, NO_QUERRY, NO_TOKEN, {cookie}, cookie.empty() ? 0 : 1);
    sendServerMessage(sockfd, message);

    // Receive the server's response
    std::string response;
    extractServerResponse(response, sockfd);

    // Extract response code and JSON content (if any)
    reply = extractJSONCode(response);
    std::string jsonResponse = extractJSONResponse(response);

    // Check if the response is empty (logout successful)
    if (jsonResponse.empty()) {
        // Reset login-related flags and clear session credentials
        login = false;
        enter = false;
        cookie.clear();
        jwt.clear();

        std::cout << "SUCCESS: " << reply << " - Logged out successfully." << std::endl;
        return;
    }

    // Handle the JSON error response from the server
    errorJSONReply(jsonResponse, reply);
}

#endif /* LOGOUT_HPP */
