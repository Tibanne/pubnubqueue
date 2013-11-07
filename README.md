# Pubnub pipelining daemon

Useful if you use a language that won't let you have persistant sockets or if
you want to only have one persistant socket, and use a UNIX OS.
This also allows you to not have to wait for the message to reach pubnub, as
sending is fully async.

This daemon will listen to a UDG socket, receive URL paths to query on pubnub,
and will query these in an async fashion while utterly ignoring anything that
comes back from pubnub, including any errors.

A sample PHP code is provided (look at test.php). Just run this on the same
machine this script is running while you have the url provided in test.php open
to see the result.


