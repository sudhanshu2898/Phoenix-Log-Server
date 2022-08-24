# Phoenix Log server #

Phoenix is a simple Microservice written in C++, Which Can be used to generate logs for various microservices.  

Phoenix Log Server generates separate logs for each connected service.

It can generate following 4 types of logs.

* Info Logs (info_log)
* Error Logs (error_log)
* Startup Logs (startup_log)
* Raw Object i.e.JSON or Text Files as log (object)

### *Step 1 - Compiling Source code: (Optional)* ⚡⌨️ ###
`g++ main.cpp -l wsock32 -o server`     
Instead of Compiling the source code you can also use server.exe

### *Step 2 - Starting Server:* 💻 ###

Start the server by executing `server.exe` File will be generated upon compilation.   
Default configuration runs the server on `localhost:7777`    
Once Server is started, you are good to go 🎉.  
You can connect to server using any websocket client or program 🎉.



***
___Optional Sections:___  
 If you are using client programs provided in `/clients` folder you can skip steps 3,4 & 5. 
***


### *Step 3 - Registering Service: (Optional)* 🔗 ###
To Register a service just connect your service via websocket to server on `localhost:7777`.

The moment you connect to the server, server will send a message asking for `service_name` (Make sure to receive the message). enter a unique string for service name Eg: `auth`

And BOOM 💥💥🎉🎉. Your service is now registered with the Log-Server. Now you can generate logs.

### *Step 4 - Generating Logs: (Optional)* 📝 ###
Generating logs is very simple, send your log in json format to server 👌 .

Eg: `{"type":"info_log", "data":"This is a simple Info log"}`

The `type` key in json is used to identify the type of log it is, and `data` is the message that you want to log.

Names for all the 4 types of logs has been mentioned above in the brackets. 
Note: object log type do not have _log appended to it. 

### *Step 5 - Generating Logs for Objects: (Optional)* 📝 ###
Generating Object logs is similar to generating normal log message.
Here we just need to add two more extra keys to our json object 👌.

* `collection` key (The name of the folder in which this object log will be generated).
* `format` key (json, txt etc..)

Eg: `{"type":"object", "collection":"users", "format":"json", "data":{"name":"sudhanshu", "city":"Mumbai"}}`

Eg: `{"type":"object", "collection":"users", "format":"txt", "data":"Hi, How are you."}`


### *Step 6 - Logging via Client Programs:* ###

Step 1: Add the PhoenixLogger class to your project (File can be found in `/clients` folder).  
Step 2: Create Instance of PhoenixLogger class, it takes 3 Arguments,

* Host Address (`localhost` in our case).
* Port Number (`7777` in our case).
* Service Name (any unique name Eg: `auth`).

Now call the log method from PhoenixLogger class, this method takes only 1 argument i.e. JSON String. (Convert JSON Object to string before passing).  

___Java Example:___

```
public class Phoenix{
    public static void main(String[] args) {
        PhoenixLogger phoenixLogger = new PhoenixLogger("localhost", 7777, "java-service");

        /** Create JSON Object */
        JSONObject infoLog = new JSONObject();
        infoLog.put("type", "info_log");
        infoLog.put("data", "This is a simple Info Log");
        
        /** Send Log to Server. */
        phoenixLogger.log(infoLog.toJSONString());


        /** Create JSON Object */
        JSONObject textObjectLog = new JSONObject();
        textObjectLog.put("type", "object");
        textObjectLog.put("collection", "users");
        textObjectLog.put("format", "txt");
        textObjectLog.put("data", "This is a simple Text Object Log");
        
        /** Send Log to Server. */
        phoenixLogger.log(textObjectLog.toJSONString());

        /** Close Socket Connection. */
        phoenixLogger.closeConnection();
    }
}
```
***

___PHP Example:___

```
<?php
    include_once("PhoenixLogger.php");

    $logger = new PhoenixLogger("localhost", 7777, "php-service");

    /** Create a JSON object */
    $message = [
        'type' => 'info_log',
        'data' => 'Quick Brown Fox Jumps over the lazy Dog.'
    ];
    $logger->log(json_encode($message));
?>
```
Note: Phoenix uses sockets for data transmission, so you need to enable SOCKETS in `php.ini`

***