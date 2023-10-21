# Matt_daemon
code a daemon

## Mandatory
- The program will have to run only with the root rights.
- Your program will have to run in the background, just like a real daemon.
- The daemon will have to listen to the port 4242.
- For your daemon’s log, you will have to create a class called Tintin_reporter.
- Every daemon action should show in a log file matt_daemon.log with timestamp (in this form [ DD / MM / YYYY - HH : MM : SS]) located in the folder /var/log/-matt_daemon/.
- Only one daemon instance should be able to run at once.
- When attempting to run a second daemon whereas one instance is already running, an error message indicating a creation/file opening on matt_daemon.lock attempt must pop.
- A matt_daemon.lock file must be created in /var/lock/ when the daemon starts.
- When the daemon shuts down, the matt_daemon.lock file must be deleted.
- The program must quit with the sending of a simple "quit" character chain on the opened socket.
- Any other character chain must be registered in the log file.
- Only 3 clients can connect simultaneously to the daemon.
- When the daemon receives a signal, it must intercept it and register it in the matt_daemon.log file with an explicit message and quit properly.

## Bonus done
- Create a graphic client to interact with the daemon.
- Advanced log archival.
- Create an authentication system to connect to the daemon (through a graphic client/remote shell).
- Add useful functions to your daemon (creation of a remote shell for instance!).

## Bonus (i think about it)
- Cypher sending and reception of the data (logically requires a client).
- Mail sending according to specific filters.
- Using advanced encryption systems (key public/private).
