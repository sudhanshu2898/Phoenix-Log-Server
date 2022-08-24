<?php


class PhoenixLogger{
    /**
     * Set MAX_MESSAGE_LENGTH to accomodate upto 1MB of Data.
     */
    const MAX_MESSAGE_LENGTH = 1048576;
    public $socket;
    function __construct($host, $port, $serviceName){
        $this->socket = socket_create(AF_INET, SOCK_STREAM, 0) or die("Failed to create Socket.");
        socket_connect($this->socket, $host, $port) or die("Failed to connect to $host:$port");
        socket_read($this->socket, self::MAX_MESSAGE_LENGTH);
        socket_write($this->socket, $serviceName, strlen($serviceName));
    }

    function log($object){
        socket_write($this->socket, $object, strlen($object)) or die('Failed to send Log.');
    }
}

?>