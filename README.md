# webtime
This command line utility program is named "webtime" as it obtains
the date and time from the header part of a web server reply.
```
% ./webtime www.google.com
Date: Wed, 12 Mar 2025 08:04:18 GMT
%
```
webtime has the following options; -
```
% ./webtime -h            
Usage:
 webtime [-D][-g][-h][-l][-p INT][-v INT][-V] SERVER_1 [.. [SERVER_N]]
 -D ...... enable debug output mode
 -g ...... switch from head request to get request
 -h ...... this help / usage information
 -l ...... enable local time information output
 -p INT .. set port number to INT - where 0 <= INT <= 65535
 -v INT .. verbosity level set to INT - where 0 <= INT <= 4
 -V ...... enable version information output
 SERVER_1 [.. [SERVER_N]] Mandatory Name or IP Address of a web server to query
%
```
