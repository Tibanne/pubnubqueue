<?php

// Browse to http://www.pubnub.com/console?channel=demo_chan&pub=demo&sub=demo&origin=pubsub.pubnub.com to see the result

function pubnub_publish($pub, $sub, $chan, $object) {
	$msg = '/publish/'.rawurlencode($pub).'/'.rawurlencode($sub).'/0/'.rawurlencode($chan).'/0/'.rawurlencode(json_encode($object));
	static $sock = null;
	if (is_null($sock)) $sock = stream_socket_client('udg:///tmp/pubnub.sock');
	return stream_socket_sendto($sock, $msg);
}

$packet = ['this is' => 'an array', 'now' => time()];

var_dump(pubnub_publish('demo','demo','demo_chan', $packet));

