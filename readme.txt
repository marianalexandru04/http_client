python3 checker.py --admin 'myadminuser:hunter2' ../path-to/client

source .venv/bin/activate
python3 checker.py -d -a 'marian.panait1206:766d189849e0' ./sources/client

python3 checker.py --script CLEANUP -d -a 'marian.panait1206:766d189849e0' ./sources/client

\username=password=POST /api/v1/tema/admin/login HTTP/1.1
Host: 63.32.125.183:8081
Content-Type: application/json
Content-Length: 58
Connection: close

{"username":"marian.panait1206","password":"766d189849e0"}HTTP/1.1 200 OK
Server: nginx/1.24.0 (Ubuntu)
Date: Wed, 14 May 2025 11:12:48 GMT
Content-Type: application/json
Content-Length: 43
Connection: close
Access-Control-Allow-Origin: *
Vary: Cookie
Set-Cookie: session=eyJhZG1pbl91c2VyX2lkIjo1MjIsInJvbGUiOiJhZG1pbiIsInVzZXJuYW1lIjoibWFyaWFuLnBhbmFpdDEyMDYifQ.aCR6sA.1jjfcLKOSrCRd0aQgz69rqfOtNs; HttpOnly; Path=/

{"message":"Admin logged in successfully"}
