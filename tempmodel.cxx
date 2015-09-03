/********************************************************
  file model.cxx
  ````````````````
  Implementing genetic algorithm as a metaheuristic for a
  bilevel formulation of the discrete network design problem.

  (c) 2015 Private
      author: Nishanth Nagendra, Mar. 2015
********************************************************/

#include <defs.h>
#include <data.h>
#include <funcs.h>

/***********************************************************************/

int model_problem(model_data *dndp, network_data *netinfo, candidate ga_cand)
{

/***Declarations***/

 XPRBexpr incr;              /* Increment to be added after x[1] */
 XPRBexpr summation;         /* Auxiliary summation expression */
 XPRBexpr increment[EL+NL];

 float points[M+1], slopes[M+1];        /* Approximation points,  Function to approximate is (x^5) */
 int i, j, k, l;           /* Iteration variables */
 float ta, xa, ba, ca, Constant;
 int r, s, r1, s1, flag = 0, count = 0;
 float temp;
 char str[100];

 //dndp->p = XPRBnewprob("TAP");
 (dndp->p).setName("TAP");

/****Approximation Points and associated slopes******/
 points[0] = 0; slopes[0] = 0;
 for (i=1; i<=M; i++) { 
    points[i] = points[i-1] + step_size;
    if (cost_function_selector) {
       slopes[i] = ( pow(points[i], 5) - pow(points[i-1], 5)) / (points[i] - points[i-1]);
    } else {
       slopes[i] = ( pow(points[i], 4) - pow(points[i-1], 4)) / (points[i] - points[i-1]);
    }
 }

 cout<<"\nPoints and slopes initialized\n";

/****VARIABLES****/

/* In case of networks having zones indicated in the dataset that do not allow traffic to pass through it, we need to handle this specially. We will have to initialize the value of link flows to zero for a given OD pair Fa_rs[OD] and for all those links which are incoming to the zonal point or outgoing from it. And also we know that there are no edges between zonal points. So an edge can atmost have one of the end points as a
zone thereby validating our IF condition j <= ZONES and next IF condition k <= ZONES to be correct checks*/

/***Flow on link a***/
 for (i=1; i<=OD; i++) {
#if ZONES 
    r = (i/ZONES);
    s = (i%ZONES);
    r = (r==0)?1 : (s==0?r : (r+1));
    s = (s==0)?ZONES : s;    
#else
    r = (i/N);
    s = (i%N);
    r = (r==0)?1 : (s==0?r : (r+1));
    s = (s==0)?N : s;    
#endif
    if (netinfo->Demand[r][s]) {
       for (l=0; l<EL; l++) {
          //j = netinfo->existing_links[l].orig;
          //k = netinfo->existing_links[l].term;
          dndp->Fa_rs[i][l] = (dndp->p).newVar("Flow");
/*#if ZONES != 0*/   /*There is a Mistake here because if j is not a zone but k can be zone. Then this will make an edge from a non zonal point to
                 a zonal point as zero rendering the destination unreachable and resulting in an infeasible constraint in the destination set
                 Mistake is probable the fact that OD is 1296 and N is 398 in case of mitte_center network because of which the calculation 
                 of OD numbers fo wrong. Needs to be handled carefully*/
          /*if (ZONES) {
             if (j <= ZONES && j != r && j != s) {  //Checks can be different in different ways like by testing if Ta is 0
                dndp->Fa_rs[i][j][k].fix(0.0); 
             } 
             if (k <= ZONES && k != r && k != s) {  //Checks can be different in different ways like by testing if Ta is 0
                dndp->Fa_rs[i][j][k].fix(0.0); 
             } 
          }*/
//#endif
       }
       
       for (l=0; l<NL; l++) { 
          if (ga_cand.binary_enc[l]) {
             //j = netinfo->new_links[l].orig; 
             //k = netinfo->new_links[l].term;
             dndp->Fa_rs[i][EL+l] = (dndp->p).newVar("Flow");
          }
       }
    } 
 }

 printf("\nDemand variables initialized\n");

/***Decision variables for linear approximation, binary variables for construction of new links and travelers on link a***/
 for (l=0; l<EL; l++) {
    //i = netinfo->existing_links[l].orig;
    //j = netinfo->existing_links[l].term;
    //if (netinfo->Ta[i][j]) {  // Added this if condition to handle cases where there are zones involved through which no traffic can
                              // pass through so the associated incoming/outgoing links have Ta as zero value.
       for (k=0; k<=M; k++) {
          dndp->x[l][k] = (dndp->p).newVar("x");
       }
    //}
    dndp->Xa[l] = (dndp->p).newVar("Travelers");
 }

 for (l=0; l<NL; l++) {
    if (ga_cand.binary_enc[l]) {
       //i = netinfo->new_links[l].orig;
       //j = netinfo->new_links[l].term;
       for (k=0; k<=M; k++) {
          dndp->x[EL+l][k] = (dndp->p).newVar("x");
       }
       dndp->Xa[EL+l] = (dndp->p).newVar("Travelers");
       dndp->Ya[l] = (dndp->p).newVar("New link", XPRB_BV);
       dndp->Ya[l].fix(1.0);
    }
 }

 cout<<"\nDecision variables declared\n";
 
/***Flow aggregation constraints***/
 for (l=0; l<EL; l++) {
    //i = netinfo->existing_links[l].orig;
    //j = netinfo->existing_links[l].term;
    summation = 0;
    for (k=1; k<=OD; k++) {
#if ZONES 
       r = (k/ZONES);
       s = (k%ZONES);
       r = (r==0)?1 : (s==0?r : (r+1));
       s = (s==0)?ZONES : s;    
#else
       r = (k/N);
       s = (k%N);
       r = (r==0)?1 : (s==0?r : (r+1));
       s = (s==0)?N : s;    
#endif
       if (netinfo->Demand[r][s]) 
          summation += dndp->Fa_rs[k][l];
    }
    (dndp->p).newCtr("Flow Aggregation", dndp->Xa[l] == summation);
 }

 for (l=0; l<NL; l++) {
    if (ga_cand.binary_enc[l]) {
       //i = netinfo->new_links[l].orig;
       //j = netinfo->new_links[l].term;
       summation = 0;
       for (k=1; k<=OD; k++) {
#if ZONES 
          r = (k/ZONES);
          s = (k%ZONES);
          r = (r==0)?1 : (s==0?r : (r+1));
          s = (s==0)?ZONES : s;    
#else
          r = (k/N);
          s = (k%N);
          r = (r==0)?1 : (s==0?r : (r+1));
          s = (s==0)?N : s;    
#endif
          if (netinfo->Demand[r][s]) 
             summation += dndp->Fa_rs[k][EL+l];
       }
    (dndp->p).newCtr("Flow Aggregation", dndp->Xa[EL+l] == summation);
    }
 }

 cout<<"\nFlow aggregation constraints declared\n";

/***Demand at origin constraints***/
#ifdef ZONES
 for (i=1; i<=ZONES; i++) {  /* for S */
#else
 for (i=1; i<=N; i++) {  /* for S */
#endif
    //summation = 0;
#ifdef ZONES
    for (j=1; j<=ZONES; j++) { /* for R */
#else
    for (j=1; j<=N; j++) { /* for R */
#endif
       //j=1; i=12;
       if (netinfo->Demand[j][i] != 0.0) {
       incr = 0;
       summation = 0;
       for (k=0; k<EL; k++) {
            r = netinfo->existing_links[k].orig;
            s = netinfo->existing_links[k].term;
            if (r != j && s != j)
               continue;
/*          if (ZONES) {
             if (netinfo->EdgeMatrix[j][k] == 1) 
                summation += dndp->Fa_rs[(N*(j-1))+i][j][k];
             if (netinfo->EdgeMatrix[k][j] == 1)
                incr += dndp->Fa_rs[(N*(j-1))+i][k][j];          
          } else {*/
             if ( (r == j) /*&& netinfo->Ta[j][s] != 0.0*/ && netinfo->EdgeMatrix[j][s] == 1) {
             #ifdef ZONES
                summation += dndp->Fa_rs[(ZONES*(j-1))+i][k];
             #else
                summation += dndp->Fa_rs[(N*(j-1))+i][k];
             #endif
             }
             if ( (s == j) /*&& netinfo->Ta[r][j] != 0.0*/ && netinfo->EdgeMatrix[r][j] == 1) {
             #ifdef ZONES
                incr += dndp->Fa_rs[(ZONES*(j-1))+i][k];          
             #else
                incr += dndp->Fa_rs[(N*(j-1))+i][k];          
             #endif
             }
         // }
       }
       for (k=0; k<NL; k++) {
          if (ga_cand.binary_enc[k]) {
             r = netinfo->new_links[k].orig;
             s = netinfo->new_links[k].term;
             if (j == r) {
             #ifdef ZONES
                summation += dndp->Fa_rs[(ZONES*(j-1))+i][EL+k];
             #else
                summation += dndp->Fa_rs[(N*(j-1))+i][EL+k];
             #endif
             }
             if (j == s) {
             #ifdef ZONES
                incr += dndp->Fa_rs[(ZONES*(j-1))+i][EL+k];
             #else
                incr += dndp->Fa_rs[(N*(j-1))+i][EL+k];
             #endif
             }
          }
       }
       (dndp->p).newCtr("origination", summation == netinfo->Demand[j][i]);
       (dndp->p).newCtr("Demand at origin", incr == 0);
       }
    }
 }

 cout<<"\nDemand at origin constraints declared\n";

/***Demand at destination constraints***/
#ifdef ZONES
 for (i=1; i<=ZONES; i++) {  /* for R */
#else
 for (i=1; i<=N; i++) {  /* for R */
#endif
    //summation = 0;
#ifdef ZONES
    for (j=1; j<=ZONES; j++) { /* for S */
#else
    for (j=1; j<=N; j++) { /* for S */
#endif
       //i=1; j=12;
       if (netinfo->Demand[i][j] != 0.0) {
       incr = 0;
       summation = 0;
       for (k=0; k<EL; k++) {
            r = netinfo->existing_links[k].orig;
            s = netinfo->existing_links[k].term;
            if (r != j && s != j)
               continue;
     /*     if (ZONES) {
             if (netinfo->EdgeMatrix[j][k] == 1) 
                incr += dndp->Fa_rs[(N*(i-1))+j][j][k];          
             if (netinfo->EdgeMatrix[k][j] == 1)
                summation += dndp->Fa_rs[(N*(i-1))+j][k][j];
          } else {*/
             if ( (r == j) /*&& netinfo->Ta[j][s] != 0.0*/ && netinfo->EdgeMatrix[j][s] == 1) {
             #ifdef ZONES
                incr += dndp->Fa_rs[(ZONES*(i-1))+j][k];          
             #else
                incr += dndp->Fa_rs[(N*(i-1))+j][k];          
             #endif
             }
             if ( (s == j) /*&& netinfo->Ta[r][j] != 0.0*/ && netinfo->EdgeMatrix[r][j] == 1) {
             #ifdef ZONES
                summation += dndp->Fa_rs[(ZONES*(i-1))+j][k];
             #else
                summation += dndp->Fa_rs[(N*(i-1))+j][k];
             #endif
             }

       }
       for (k=0; k<NL; k++) {
          if (ga_cand.binary_enc[k]) {
             r = netinfo->new_links[k].orig;
             s = netinfo->new_links[k].term;
             if (r == j) {
             #ifdef ZONES
                incr += dndp->Fa_rs[(ZONES*(i-1))+j][EL+k];
             #else
                incr += dndp->Fa_rs[(N*(i-1))+j][EL+k];
             #endif
             }
             if (s == j) {
             #ifdef ZONES
                summation += dndp->Fa_rs[(ZONES*(i-1))+j][EL+k];
             #else
                summation += dndp->Fa_rs[(N*(i-1))+j][EL+k];
             #endif
             }
          }
       }
       (dndp->p).newCtr("Demand at destination", summation == netinfo->Demand[i][j]);
       (dndp->p).newCtr("destination", incr == 0);
       }
    }
 }

 cout<<"\nDemand at destination constraints declared\n";

/***Flow conservation constraints***/
 for (i=1; i<=OD; i++) {
#if ZONES 
    r = (i/ZONES);
    s = (i%ZONES);
    r = (r==0)?1 : (s==0?r : (r+1));
    s = (s==0)?ZONES : s;    
#else
    r = (i/N);
    s = (i%N);
    r = (r==0)?1 : (s==0?r : (r+1));
    s = (s==0)?N : s;    
#endif
    if (netinfo->Demand[r][s] != 0.0) {
    //r=1; s=12; i=12;
    for (j=1; (j<=N) /*&& (j!=r) && (j!=s)*/; j++) {
       if (j!=r && j!=s) { 
       summation = 0;
       incr = 0;
       for (k=0; k<EL; k++) {
            r1 = netinfo->existing_links[k].orig;
            s1 = netinfo->existing_links[k].term;
      /*    if (ZONES) {
             if (netinfo->EdgeMatrix[j][k] == 1) 
                incr += dndp->Fa_rs[i][j][k];
             if (netinfo->EdgeMatrix[k][j] == 1)
                summation += dndp->Fa_rs[i][k][j];
          } else {*/
          if ((s1 == j) /*&& netinfo->Ta[r1][j] != 0.0*/ && netinfo->EdgeMatrix[r1][j] == 1)
             summation += dndp->Fa_rs[i][k];
          if ((r1 == j) /*&& netinfo->Ta[j][s1] != 0.0*/ && netinfo->EdgeMatrix[j][s1] == 1)
             incr += dndp->Fa_rs[i][k];
         // }
       }
       for (k=0; k<NL; k++) {
          if (ga_cand.binary_enc[k]) {
             r1 = netinfo->new_links[k].orig;
             s1 = netinfo->new_links[k].term;
             if (j == r1) {
                incr += dndp->Fa_rs[i][EL+k];
             }
             if (j == s1) {
                summation += dndp->Fa_rs[i][EL+k];
             }
          }
       }
       (dndp->p).newCtr("Flow conservation", summation == incr);
    } }
    }
 }


 summation = 0;

 cout<<"\nBudget constraints declared\n";

/***Budget constraint***/
 for (i=0; i<NL; i++) {
    if (ga_cand.binary_enc[i]) {
       j = netinfo->new_links[i].orig;
       k = netinfo->new_links[i].term;
       summation += (netinfo->ba[j][k] * dndp->Ya[i]);
       //(dndp->p).newCtr("bigM constraint", dndp->Xa[EL+k] <= (bigM * (dndp->Ya[i])) );
       (dndp->p).newCtr("bigM constraint", dndp->Xa[EL+i] <= (bigM * (dndp->Ya[i])) );
    }
 }

 (dndp->p).newCtr("Budget constraint", summation <= Budget);   

 cout<<"\nBefore objective\n";

/****OBJECTIVE****/
/* for (i=1; i<=N; i++) {
    printf("\n");
    for (j=1; j<=N; j++) {
    if (netinfo->Ta[i][j] != 0.0 && netinfo->EdgeMatrix[i][j] == 1) { //Condition to determine whether to have constraints for this edge or not*/
 for (l=0; l<EL; l++) {
       i = netinfo->existing_links[l].orig;
       j = netinfo->existing_links[l].term;
       cout<<i<<" "<<j<<"\t";
       ta = netinfo->Ta[i][j];
       //xa = Xa[i][j];
       ca = netinfo->Ca[i][j];
       ba = netinfo->Ba[i][j];
       Constant = ( (ta * ba)/(5 * pow(ca, 4)) );
       //dndp->increment[i][j] = 0;
       increment[l] = 0;
       if (ta) {
       for (k=2; k<=M; k++) {
          increment[l] += ( (slopes[k] - slopes[k-1]) * dndp->x[l][k] );
          //p.newCtr("Increments", x[i][j][1] <= (x[i][j][k] + points[k-1]) );
          (dndp->p).newCtr("Increments", dndp->Xa[l] <= (dndp->x[l][k] + points[k-1]) );
       }
       if (cost_function_selector == 2) {
          summation += (ta * dndp->Xa[l]) + (Constant * (0 + (slopes[1]*dndp->Xa[l]) + increment[l]) );  /* 0 represents function value at X{0} which is                                                                                                zero since the non linear function here is x^5*/
       } else {
          //summation += (ta * Xa[i][j]) + ( (0.008/5) * (0 + (slopes[1]*x[i][j][1]) + increment[i][j]) );
          summation += (ta * dndp->Xa[l]) + ( (0.008/5) * (0 + (slopes[1]*dndp->Xa[l]) + increment[l]) );
       }
    } 
   }
 //}  
 printf("\n");
 for (l=0; l<NL; l++) {
    if (ga_cand.binary_enc[l]) {
       i = netinfo->new_links[l].orig;
       j = netinfo->new_links[l].term;
       ta = netinfo->Ta[i][j];
       ca = netinfo->Ca[i][j];
       //ba = netinfo->Ba[i][j];
       //ba = 0.15;  //Temporarily commented out for mitte center dataset
       ba = 1.0;
       Constant = ( (ta * ba)/(5 * pow(ca, 4)) );
       increment[EL+l] = 0;
       for (k=2; k<=M; k++) {
          increment[EL+l] += ( (slopes[k] - slopes[k-1]) * dndp->x[EL+l][k] );
          //p.newCtr("Increments", x[i][j][1] <= (x[i][j][k] + points[k-1]) );
          (dndp->p).newCtr("Increments", dndp->Xa[EL+l] <= (dndp->x[EL+l][k] + points[k-1]) );
       }
//#ifdef _CODE
       if (cost_function_selector == 2) {
          summation += (ta * dndp->Xa[EL+l]) + (Constant * (0 + (slopes[1]*dndp->Xa[EL+l]) + increment[EL+l]) );  /* 0 represents function value at X{0} which is                                                                                                zero since the non linear function here is x^5*/
       } else {
//#endif 
          //summation += (ta * Xa[i][j]) + ( (0.008/5) * (0 + (slopes[1]*x[i][j][1]) + increment[i][j]) );
          summation += (ta * dndp->Xa[EL+l]) + ( (0.008/5) * (0 + (slopes[1]*dndp->Xa[EL+l]) + increment[EL+l]) );
       }
    }  
 }
       
 (dndp->p).setObj((dndp->p).newCtr("OBJ", summation) ); /* Define & set the obj. function */ 

/****SOLVING****/
 (dndp->p).setSense(XPRB_MINIM);
 //(dndp->p).lpOptimize("");         /* Solve the LP-problem */
 //p.mipOptimize("");         /* Solve the LP-problem */
 (dndp->p).exportProb(XPRB_LP);

 return 0;  
}
