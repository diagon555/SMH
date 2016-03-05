<?php

class Client
{
	private $login;
	private $socket;
	private $ip;
	private $logined = false;
	
	public function __construct($socket)
	{
		$this->socket = $socket;
	}
	
	public function Logined()
	{
		return $this->logined;
	}
	
	public function Auth($login, $token)
	{
		if(!$token || $token == "456")
		{
			$this->login = $login;
			$this->logined = true;
			return true;
		}
		else
		{
			return false;
		}
	}
	
	public function SetLogin($login)
	{
		$this->login = $login;
	}
	
	public function GetLogin()
	{
		return $this->login;
	}
	
	public function SetIP($ip)
	{
		$this->ip = $ip;
	}
	
	public function GetIP()
	{
		return $this->ip;
	}

	public function Socket()
	{
		return $this->socket;
	}
}

function socket_client_check(&$socket, &$connects, &$devices)
{
	$connects_read = array();
	foreach($connects as $connect)
	{
		$connects_read[] = $connect->Socket();
	}
	$connects_read[] = $socket;
	
	$write = null;
	$except = null;
    if(socket_select($connects_read, $write = NULL, $except = NULL, 0, 1000) > 0) //ожидаем сокеты доступные для чтения (без таймаута)
	{
		if (in_array($socket, $connects_read)) {//есть новое соединение
			if (($new_client = socket_accept($socket)) === false) {
				echo "socket_accept() failure: reason: " . socket_strerror(socket_last_error($socket)) . "\n\r";
				return 0; //!!!
			}

			$client = new Client($new_client);
			$ip = "";
			if(socket_getpeername($new_client, $ip))
			{
				$client->SetIP($ip);
			}
			$connects[] = $client;//добавляем его в список необходимых для обработки

			$sock = $client->Socket();
			$msg = "LOGIN TOKEN?\n\r";
			socket_write($client->Socket(), $msg, strlen($msg));
		}

		foreach ($connects as $key => $client) { // for each client       
			if (in_array($client->Socket(), $connects_read)) {
				if (false === ($buf = socket_read($client->Socket(), 2048, PHP_NORMAL_READ))) {
					echo "socket_read() failure: reason: " . socket_strerror(socket_last_error($client->Socket())) . "\n\r";
					return 0; //!!!
				}
				if (!$buf = trim($buf)) {
					return 1;
				}
				
				$talkback = "";
				if($client->Logined())
				{
					$cmd = strtolower(shift_word($buf));

					if ($cmd == 'device' || $cmd == 'dev') {
						$devname=strtolower(shift_word($buf));
						$finded = false;
						$all = ($devname == "all");
						foreach($devices as $device)
						{
							if($all || $device->GetName() == $devname)
							{
								if(!$device->Logined())
									continue;

								$finded = true;
								
								$to_sent = "CLIENT " . $client->GetLogin() . " $buf\n\r";
								socket_write($device->Socket(), $to_sent, strlen($to_sent));
								if(!$all) break;
							}
						}
						if(!$finded)
						{
							$talkback = "ERR device was not found\n\r";
						}
					}
					elseif ($cmd == 'client' || $cmd == 'clt') {
						$cltname=strtolower(shift_word($buf));
						$finded = false;
						$all = ($devname == "all");
						foreach($connects as $cur_client)
						{
							if($all || $cur_client->GetLogin() == $cltname)
							{
								if(!$cur_client->Logined() || $cur_client->GetLogin() == $client->GetLogin())
									continue;
								
								$finded = true;
								
								$to_sent = "CLIENT " . $client->GetLogin() . " $buf\n\r";
								socket_write($cur_client->Socket(), $to_sent, strlen($to_sent));
								if(!$all) break;
							}
						}
						if(!$finded)
						{
							$talkback = "ERR client was not found\n\r";
						}
					}
					elseif ($cmd == 'devlist') {
						$talkback = "Devices list:\n\r";
						if($devices)
						{
							foreach($devices as $device)
							{
								if($device->Logined())
									$talkback .= $device->GetName() . " " . $device->GetIP() . "\n\r";
							}
						}
						else
						{
							$talkback .= "<no devices>\n\r";
						}
					}
					elseif ($cmd == 'userlist') {
						$talkback = "User list:\n\r";
						foreach($connects as $connect)
						{
							if($connect->Logined())
								$talkback .= $connect->GetLogin() . " " . $connect->GetIP() . "\n\r";
						}
					}
					elseif ($cmd == 'quit') {
						socket_close($client->Socket());
						unset($connects[$key]);
						return 1;
					}
					elseif ($cmd == 'shutdown' || $cmd == 'sd') {
						//socket_close($client->Socket()); //Closed at the ent of main file
						return 0;
					}
					else
					{
						$talkback = "(dev <name> <command>) devlist userlist quit shutdown\n\r";
					}
				}
				else
				{
					$cred = split(" ", $buf);
					echo "login: $cred[0]:$cred[1]\n\r";
					
					$exists = false;
					foreach($connects as $connect)
					{
						if($connect->GetLogin() == $cred[0])
						{
							$exists = true;
							break;
						}
					}
					
					if($except)
					{
						$talkback = "ERR ALREDY LOGINED LOGIN TOKEN?\n\r";
					}
					elseif($client->Auth($cred[0], $cred[1]))
					{
						$talkback = "HELLO $cred[0]\n\r";
					}
					else
					{
						$talkback = "ERR LOGIN TOKEN?\n\r";
					}
				}
								
				if($talkback)
					socket_write($client->Socket(), $talkback, strlen($talkback));
			}
		}
	}
	
	return 1;
}



?>
