# Triples model for the BPMP presented in
# The Backhaul Profit Maximization Problem: Optimization Models and Solution Procedures by Dong et al.
# https://doi.org/10.1287/ijoo.2022.0071

param n; #total number of nodes

set N := 1..n; # the node set including starting and end nodes
set A := {i in N diff {n}, j in N diff ({1} union {i})}; # the arc set
set T := {i in N diff {n}, j in N diff {1}, k in N diff {1,n}: i<>j and i<>k and j<>k}; # the triples set

param p; 		 # price ($ per mile per ton)
param c; 		 # cost  ($ per mile per ton)
param d{i in N, j in N}; # the weight of the request (in tons) from node i to node j
param w{k in N, l in N}; # the weight of the request (in tons) from node k to node l
param DIS; 		 # the maximum distance (in miles) the vehicle can travel (note that this parameter is called D in the paper)
param Q; 		 # the vehicle capacity in tons (note that this parameter is called W in the paper)
param v;	 	 # the vehicle weight in tons

var x{(i,j) in A} binary; 		# x[i,j] = 1 if the vehicle travels on arc (i, j); and zero otherwise
var u{(i,j,k) in T} >=0;  		# flow (tons of cargo) on paths comprised of arc (i, k) follwed by a path from k to j
var y{(k,l) in A} binary; 		# y[k,l] = 1 if the delivery request (k,l) is accepted; and zero otherwise
var s{i in N diff {1}} >= 1 <= n-1;	# sequence lables for MTZ constraints


maximize profit: 
(sum{(i,j) in A} p*d[i,j]*w[i,j]*y[i,j] -
sum{(i,j) in A} c*d[i,j]*(w[i,j]*y[i,j] +
sum{k in N: (i,k,j) in T} u[i,k,j] +sum{k in N: (k,j,i) in T} u[k,j,i] -
sum{k in N: (i,j,k) in T} u[i,j,k]) -
c*v*(sum{(i,j) in A} d[i,j]*x[i,j]));

s.t. c2: sum{j in N: (1,j) in A} x[1,j] = 1; 
s.t. c3: sum{i in N: (i,n) in A} x[i,n] = 1;
s.t. c4 {k in N diff {1,n}}: sum{i in N:(i,k) in A} x[i,k] = sum{j in N:(k,j) in A} x[k,j];
s.t. c5 {i in N diff {1,n}, j in N diff {1,n}: i <> j}: s[i] - s[j] + (n-1)*x[i,j] +(n-3)*x[j,i] <= n-2;
s.t. c6: sum{(i,j) in A} d[i,j]*x[i,j] <= DIS;

s.t. TriplesConstraint {(i,j) in A}: y[i,j]*w[i,j] + sum{k in N: (i,k,j) in T} u[i,k,j] + sum{k in N: (k,j,i) in T} u[k,j,i] <= Q*x[i,j] + sum{k in N: (i,j,k) in T} u[i,j,k];

s.t. cut1  {k in N:k<>1 and k<>n}: sum{i in N:i<>k and i<>n} x[i,k] <= 1;
s.t. cut2a {i in N diff {n}}: sum{(i,j) in A} w[i,j]*y[i,j] <= Q;
s.t. cut2b {j in N diff {1}}: sum{(i,j) in A} w[i,j]*y[i,j] <= Q;

