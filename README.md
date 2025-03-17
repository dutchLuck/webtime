# webtime
This command line utility program is named "webtime" as it obtains
the date and time from the header part of a web server reply.
```
% ./webtime www.google.com                                                                                     
Date: Mon, 17 Mar 2025 10:04:29 GMT
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
Visual comparison of the time on the local computer and the time
on the web server can be made by using the -l option. For example; -
```
% ./webtime -l www.google.com
Local Host GMTime: Mon Mar 17 10:04:35 2025
Server Date: Mon, 17 Mar 2025 10:04:35 GMT
%
```
