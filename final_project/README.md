#

VAR G1 = 1; VAR G2 = 2;		# Global Variables
FUNC F1(a);			# Function declaration
FUNC F2(a,b,c) = F1(a)+F1(b)*F1(c);	# Function definition
FUNC F1(a) = a*G2;		# Function definition
Begin
    VAR L1 = In();
    VAR L2 = F2(L1, L1+G1, L1+G2);
    Out(L2)
End
