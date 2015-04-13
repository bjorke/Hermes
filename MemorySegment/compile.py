from subprocess import call
call(["gcc","write.c","-o","write.out"])
call(["gcc","read.c","-o","read.out"])
call(["gcc","release.c","-o","release.out"])
