from subprocess import call
call(["gcc","-c","Connection.c"])
call(["gcc","-c","Checksum.c"])
call(["gcc","-o","executable.o","Connection.o"])
