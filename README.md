# webtime
This command line utility program is named "webtime" as it obtains
the (UTC) date and time from the header part of a web server reply.
```
% ./webtime www.google.com 
Date: Wed, 19 Mar 2025 23:49:49 GMT
%
```
webtime has the following options; -
```
% ./webtime -h                                                                                            
Usage:
 webtime [-D][-g][-h][-i INT][-l][-p INT][-v INT][-V] SERVER_1 [.. [SERVER_N]]
 -D ...... enable debug output mode
 -g ...... switch from head request to get request
 -h ...... this help / usage information
 -i INT .. set IP version to 4, 6 or 46 - where 0 <= INT <= 64
 -l ...... enable local time information output
 -p INT .. set port number to INT - where 0 <= INT <= 65535
 -v INT .. verbosity level set to INT - where 0 <= INT <= 4
 -V ...... enable version information output
 SERVER_1 [.. [SERVER_N]] Mandatory Name or IP Address of a web server to query
%
```
Further information about verbosity level follows; -
```
  -v 0 .. Output only the date and time 
  -v 1 or no -v option specified .. Default verbosity level. It labels the date string.
  -v 2 .. Adds the server name into the output string provided by -v1.
  -v 3 .. Displays the request lines sent to the server and the reply lines from the server.
  -v 4 .. Adds a count of the number of characters in each server reply line.
```

Visual comparison of the time on the local computer and the time
on the web server can be made by using the -l option. For example; -
```
% ./webtime -l www.google.com
Local Host GMTime: Wed Mar 19 23:51:39 2025
Server Date: Wed, 19 Mar 2025 23:51:39 GMT
%
```
If multiple servers are specified then the -v 2 verbosity level provides
the following; -
```
% ./webtime -v2 www.microsoft.com www.google.com
"www.microsoft.com": Date: Wed, 19 Mar 2025 23:40:25 GMT
"www.google.com": Date: Wed, 19 Mar 2025 23:40:25 GMT
%
```
