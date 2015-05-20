from subprocess import call
call(["gcc","Connection.c","-o","Connection.out"])
call(["gcc","Read.c","-o","Read.out"])
call(["gcc","Release.c","-o","Release.out"])
