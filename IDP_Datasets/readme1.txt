Various networks in Berlin's area provided by Rolf Möhring (TU Berlin)
Andreas Schulz (MIT), and Nicolas Stier-Moses (Columbia University)
with the assistance of Stefan Gnutzmann (DaimlerChrysler). These
networks were used, among other things, in the paper by O. Jahn, R.H.
Möhring, A.S. Schulz, and N. Stier-Moses titled "System-Optimal
Routing of Traffic Flows with User Constraints in Networks with
Congestion" (Operations Research, 53:4, 600-616, 2005). 


The format of these files is described below:

Node structure (.nod): node_number x y

Node numbers conversion (.gra.nodid): external_node_number internal_node_number

Demand original file (using external node numbers)  (.dem): id external_node_number1 external_node_number2 value

Demand converted file(same as above but with internal node numbers) (.OD): 
	id internal_node_number1 internal_node_number2 value

Link structure (.arc): 
   id node_number1 node_number2 length capacity type alpha beta gamma

where type is 0 for artificial and 1 for a real link and congestion
There is a bit of unclarity about the interpretation of the link cost functions. 
They are either:
alpha * ( 1.0 + beta * (flow/gamma)^4 )
or
alpha + beta * (flow/gamma)^4 

In all the networks there is a constant ratio beween beta and alpha for real links. (2 for Berlin, 1 for all others)
The second interpretation seems more likely, so it has been used for the conversion to the TNTP format.



--
Rolf Möhring, TU Berlin
Andreas Schulz, MIT
Nicolas Stier-Moses, Columbia University


