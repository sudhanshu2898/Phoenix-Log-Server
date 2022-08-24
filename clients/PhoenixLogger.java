import  java.net.*;
import  java.io.*;

public class PhoenixLogger {
    Socket socket = null;
    InputStream inputStream = null;
    OutputStream outputStream = null;

    public PhoenixLogger(String host, int port, String serviceName) {
        try{
            /** Connect to Log Server */
            socket = new Socket(host, port);

            /**
             * Initialize Input Stream and Read incoming data from server.
             * Just read the input no need to take any action
             * */
            inputStream = socket.getInputStream();
            inputStream.read(new byte[1024]);

            /**  Prepare Output Stream and send serviceName to log server. */
            outputStream = socket.getOutputStream();
            outputStream.write(serviceName.getBytes("UTF8"));
        }catch (Exception e){
            System.out.println(e.getMessage());
        }
    }

    void log(String object){
        if(socket.isConnected()){
            try{
                outputStream.write(object.getBytes("UTF8"));
            }catch (Exception e){
                System.out.println(e.getMessage());
            }
        }
    }

    void closeConnection(){
        try{
            socket.close();
            inputStream.close();
            outputStream.close();
        }catch (Exception e){
            System.out.println(e.getMessage());
        }
    }

}
