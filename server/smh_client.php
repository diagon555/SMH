<?php

class Client
{
	private $login;
	private $socket;
	
	public function Client($socket)
	{
		$this->socket = $socket;
	}
	
	public function SetLogin($login)
	{
		$this->login = $login;
	}
	
	public function GetLogin()
	{
		return $login;
	}
	
	public function Socket()
	{
		return $socket;
	}
}

function socket_client_check($socket, &$connects, &$devices)
{
	$connects_read = $connects;
	$connects_read[] = $socket;
	
	$write = null;
	$except = null;
    if(socket_select($connects_read, $write, $except, NULL) > 0) //������� ������ ��������� ��� ������ (��� ��������)
	{
		if (in_array($socket, $connects_read)) {//���� ����� ����������
			if (($new_client = socket_accept($socket)) === false) {
				echo "socket_accept() failure: reason: " . socket_strerror(socket_last_error($socket)) . "\n";
				return 0; //!!!
			}

			$connects[] = $new_client;//��������� ��� � ������ ����������� ��� ���������
			$key = array_keys($connects, $new_client);
			/* Enviar instrucciones. */
			$msg = "\nHello! \r\n" .
			"Client: {$key[0]}\r\n" .
			"Type 'quit'. Or 'shutdown'.\r\n";
			socket_write($new_client, $msg, strlen($msg));
		}

		foreach ($connects as $key => $client) { // for each client       
			if (in_array($client, $connects_read)) {
				if (false === ($buf = socket_read($client, 2048, PHP_NORMAL_READ))) {
					echo "socket_read() failure: reason: " . socket_strerror(socket_last_error($client)) . "\n";
					return 0; //!!!
				}
				if (!$buf = trim($buf)) {
					return 1;
				}
				if ($buf == 'quit') {
					unset($connects[$key]);
					socket_close($client);
					return 1;
				}
				if ($buf == 'shutdown' || $buf == 'sd') {
					socket_close($client);
					return 0;
				}
				
				$talkback = "Client {$key}: Say '$buf'.\r\n";
				socket_write($client, $talkback, strlen($talkback));
				socket_write($devices[0], $talkback, strlen($talkback));
				echo "Client {$key}: <$buf>\n";
			}
		}
	}
	
	return 1;
}



?>
