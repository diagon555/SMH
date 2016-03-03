<?php


// return socket
function smh_create_socket($address, $port)
{
	//Creating client socket
	if (($socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) === false) {
		echo "socket_create() failure: reason: " . socket_strerror(socket_last_error()) . "\n";
		return false;
	}

	if (socket_bind($socket, $address, $port) === false) {
		echo "socket_bind() failure: reason: " . socket_strerror(socket_last_error($socket)) . "\n";
		return false;
	}

	if (socket_listen($socket) === false) {
		echo "socket_listen() failure: reason: " . socket_strerror(socket_last_error($socket)) . "\n";
		return false;
	}
	
	return $socket;
}

?>
