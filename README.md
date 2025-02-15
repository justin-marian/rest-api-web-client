# 📌 REST API Web Client

This project is a **REST API web client** that allows users to communicate with a remote server using **socket connections and HTTP requests**. It provides functionalities for **user authentication, library access, and book management**, using **GET, POST, and DELETE requests**. The client is built in **C++** and interacts with the server through **manually constructed HTTP requests**, making it lightweight and efficient.

---

## 🚀 Features

### User Authentication

- **register_credentials()** – Registers a new user by sending user credentials to the server.
- **login()** – Authenticates the user by verifying login credentials with the server and retrieves a session cookie.
- **logout()** – Terminates the current session and clears authentication tokens and cookies.

### Library Access

- **enter_library()** – Grants access to the virtual library by sending an authentication request and retrieves a JWT token for further operations.

### Book Management

- **get_books()** – Retrieves a list of all books available in the library.
- **get_book()** – Retrieves detailed information about a specific book using its unique ID.
- **add_book()** – Adds a new book to the library by sending book details to the server.
- **del_book()** – Deletes a book from the library using its unique ID.

---

## 📌 HTTP Request Functions

### **1️⃣ GET() – Sends a GET Request**

**Purpose:**  
Retrieves data from the server, such as a list of books or details of a specific book.

**Parameters:**  

- **host** – The server's hostname or IP address.
- **url** – The API endpoint to which the request is sent.
- **query_params** – Optional query parameters to append to the URL.
- **token** – JWT token for authentication (optional).
- **cookies** – A vector of session cookies (optional).
- **cookies_count** – The number of cookies to include in the request.

**Process:**  

1. Constructs the GET request line, appending the URL and any query parameters if provided.
2. Adds necessary HTTP headers, including:
   - **Host**: The server's hostname or IP address.
   - **Authorization**: Bearer `token` (if authentication is required).
   - **Cookie**: <session_data> (if session-based authentication is needed).
3. Terminates the request with an empty line to signal the end of headers.
4. Sends the request over the established socket connection and waits for the server's response.

---

### **2️⃣ POST() – Sends a POST Request**

**Purpose:**  
Sends data to the server, such as user authentication credentials or book details.

**Parameters:**

- **host** – The server's hostname or IP address.
- **url** – The API endpoint to which the request is sent.
- **token** – JWT token for authentication (optional).
- **content_type** – The format of the request body (e.g., "application/json").
- **body** – A JSON string containing the request data.
- **body_fields_nr** – The number of fields in the request body.
- **cookies** – A vector of session cookies (optional).
- **cookies_count** – The number of cookies to include in the request.

**Process:**  

1. Constructs the POST request line, specifying the HTTP method and URL.
2. Adds necessary HTTP headers, including:
   - **Host**: The server's hostname or IP address.
   - **Authorization**: Bearer `token` (if authentication is required).
   - **Content-Type**: Specifies the format of the request body (e.g., "application/json").
   - **Content-Length**: The length of the JSON body.
3. Includes the request body (JSON payload) in the request.
4. Sends the request over the socket connection and processes the server's response.

---

### **3️⃣ DELETE() – Sends a DELETE Request**

**Purpose:**  
Removes a resource from the server, such as deleting a book from the library.

**Parameters:**

- **host** – The server's hostname or IP address.
- **url** – The API endpoint to which the request is sent.
- **token** – JWT token for authentication (optional).
- **content_type** – The format of the request body (usually empty for DELETE requests).
- **body** – A JSON string containing the request data (usually empty for DELETE requests).
- **body_fields_nr** – The number of fields in the request body.
- **cookies** – A vector of session cookies (optional).
- **cookies_count** – The number of cookies to include in the request.

**Process:**  

1. Constructs the DELETE request line, specifying the HTTP method and URL.
2. Adds necessary HTTP headers, including:
   - **Host**: The server's hostname or IP address.
   - **Authorization**: Bearer `token` (if authentication is required).
   - **Content-Type**: Specifies the format of the request body (if applicable).
   - **Content-Length**: The length of the JSON body (if applicable).
3. Sends the request over the socket connection and processes the server's response.

---

## 📝 Server Response Handling

### **4️⃣ extractServerResponse() – Receives and Processes the Server Response**

**Purpose:**  
Retrieves the server's response from the socket and processes it.

**Process:**  

1. Reads the server's response from the socket connection.
2. Parses the HTTP response to extract the status code, headers, and body.
3. Stores the response for further processing.

---

### **5️⃣ errorJSONReply() – Parses and Handles JSON Errors**

**Purpose:**  
Extracts error messages from JSON responses and handles them appropriately.

**Process:**  

1. Parses the JSON response to check for error messages.
2. If an error is found, formats and prints the error message to the user.

---

### **6️⃣ extractResponseCode() – Extracts the HTTP Status Code**

**Purpose:**  
Retrieves the HTTP status code from the server's response.

**Process:**  

1. Parses the server's response to extract the HTTP status code (e.g., 200 OK, 404 Not Found).
2. Returns the status code for further processing.

---

### **7️⃣ extractJSONResponse() – Extracts the JSON Response Body**

**Purpose:**  
Retrieves the JSON-formatted data from the server's response.

**Process:**  

1. Parses the server's response to extract the JSON body.
2. Returns the JSON data for further processing.

---

## 🔒 Authentication Functions

### **8️⃣  register_credentials() – Registers a New User**

**Purpose:**  
Creates a new user account by sending registration details to the server.

**Process:**  

1. Prompts the user to enter a username and password.
2. Formats the credentials into a JSON object.
3. Sends a POST request to the server with the registration details.
4. Processes the server's response to confirm successful registration.

---

### **9️⃣ login() – Logs in a User**

**Purpose:**  
Authenticates the user by verifying login credentials with the server.

**Process:**

1. Prompts the user to enter a username and password.
2. Formats the credentials into a JSON object.
3. Sends a POST request to the server to authenticate the user.
4. Extracts the session cookie from the server's response for future requests.

---

### **🔟 logout() – Logs out a User**

**Purpose:**  
Terminates the current session and invalidates authentication credentials.

**Process:**  

1. Sends a GET request to the server's logout endpoint.
2. Clears the session cookie and JWT token, effectively logging the user out.

---

## 📚 Library Access

### **1️⃣1️⃣ enter_library() – Grants Access to the Virtual Library**

**Purpose:**  
Allows the user to enter the virtual library by sending an authentication request.

**Process:**  

1. Sends a GET request to the server with the authentication token.
2. Extracts and stores the JWT token from the server's response for future library operations.

---

## 📖 Book Management

### **1️⃣2️⃣ get_books() – Fetches a List of All Books**

**Purpose:**  
Retrieves a list of all books available in the library.

**Process:**  

1. Sends a GET request to the server to retrieve all books.
2. Parses the server's response to display the list of books.

---

### **1️⃣3️⃣ get_book() – Fetches a Specific Book**

**Purpose:**  
Retrieves detailed information about a specific book using its unique ID.

**Process:**  

1. Prompts the user to enter the book's ID.
2. Sends a GET request to the server to fetch the book's details.
3. Parses the server's response to display the book's information.

---

### **1️⃣4️⃣ add_book() – Adds a New Book**

**Purpose:**  
Adds a new book to the library by sending book details to the server.

**Process:**  

1. Prompts the user to enter the book's details (title, author, genre, page count, publisher).
2. Formats the book details into a JSON object.
3. Sends a POST request to the server with the book data.
4. Processes the server's response to confirm successful addition.

---

### **1️⃣5️⃣ del_book() – Deletes a Book**

**Purpose:**  
Removes a book from the library using its unique ID.

**Process:**  

1. Prompts the user to enter the book's ID.
2. Sends a DELETE request to the server to remove the book.
3. Processes the server's response to confirm successful deletion.

---
