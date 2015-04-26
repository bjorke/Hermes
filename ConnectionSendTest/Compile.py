from subprocess import call
call(["gcc","Memory.c","-o","Memory.out"])
call(["gcc","Send.c","-o","Send.out"])
call(["gcc","Deallocate.c","-o","Deallocate.out"])
