#!/usr/bin/env php

<?php

include("smh_base.php");
include("smh_client.php");
include("smh_device.php");

//error_reporting(E_ALL);
error_reporting(E_ALL && !E_NOTICE);
set_time_limit(0);
ob_implicit_flush(); //autoflush

$device_socket = array(
	'address'=> "0.0.0.0",
	'port'=> 5505
);

$client_socket = array(
	'address'=> "0.0.0.0",
	'port'=> 5506
);

$socket_device = smh_create_socket($device_socket['address'], $device_socket['port']);
$socket_client = smh_create_socket($client_socket['address'], $client_socket['port']);

if(!$socket_device)
{
	exit;
}
if(!$socket_client)
{
	socket_close($socket_device);
	exit;
}

$devices_connects = array();
$client_connects = array();

$cnt = 500;

while(true)
{
	if(!socket_device_check($socket_device, $devices_connects, $client_connects))
		break;
	if(!socket_client_check($socket_client, $client_connects, $devices_connects))
		break;
	//echo '.';
	
	
	//if(!$cnt--) break;
}

echo "closing sockets\n";

/*
foreach($devices_connects as $connect)
{
	$sock = $connect->Socket();
	echo "close: <$sock>\n";
	socket_close($sock);
}
foreach($client_connects as $connect)
{
	$sock = $connect->Socket();
	echo "close: <$sock>\n";
	socket_close($sock);
}
*/

socket_close($socket_device);
socket_close($socket_client);
echo "quit\n";


?>