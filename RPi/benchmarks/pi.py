
# Import required modules
from    time       import  time

def piApprox(num):
    pi=4.0
    k=1.0
    est=1.0
    while 1<num:
        k+=2
        est=est-(1/k)+1/(k+2)
        num=num-1
        k+=2
    return pi*est

RES = 100000000
print( "\n#############################" );
print( " 	  PYTHON CODE 	     " );
print( "#############################\n" );
print( "Approximating pi using {} iterations".format(RES) );
startTime = time()
p = piApprox( RES )
endTime = int((time()-startTime)*1000)
print( "Time taken {:d} seconds {:d} milliseconds".format(endTime/1000, endTime%1000) )
print( "pi = {:6f}\n".format(p) )
