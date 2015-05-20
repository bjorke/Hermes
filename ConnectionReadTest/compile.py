from subprocess import call
call(["gcc","Connection.c","-o","Connection.out"])
call(["gcc","Read.c","-o","Read.out"])
call(["gcc","Deallocate.c","-o","Deallocate.out"])
