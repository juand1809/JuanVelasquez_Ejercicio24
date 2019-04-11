#include <fstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <cmath>

using namespace std; 

float *read_file(string filename, int *n_points);
void model(float *y, float*x, float *c, int n_points, int poly_degree);
float loglikelihood(float *x_obs, float *y_obs, int n_points, float *c, int poly_degree);
float logprior(float *c, int poly_degree);
void MCMC_polynomial(float *x_obs, float *y_obs, int n_points, int n_steps, int poly_degree);

int main(){
  float *x=NULL;
  float *y=NULL;
  int n_x=0;
  int n_y=0;

  x = read_file("valores_x.txt", &n_x);
  y = read_file("valores_y.txt", &n_y);
  
  MCMC_polynomial(x, y, n_x, 1000000, 3);
  
  return 0;
}

void model(float *y, float*x, float *c, int n_points, int poly_degree){
    int i,j;
    for (i=0; i < n_points; i++){
        y[i] = 0;
    }
    
    for (i=0;i<n_points;i++){
        for(j=0; j <= poly_degree;j++){
            y[i]=y[i]+c[j] + pow(x[i],j);
        }
    }   
}

float loglikelihood(float *x_obs, float *y_obs, int n_points, float *c, int poly_degree){
    float * y_modelo;
    float logprob = 0.0;
    float sigma = 0.1;
    int i;
    y_modelo = new float[n_points];
    model(y_modelo,x_obs,n_points,c,poly_degree);
    
    for (i = 0; i < n_points; i++){
        logprob = logprob - 0.5*pow((y_modelo[i] - y_obs[i])/sigma,2);
    }
       
    delete [] y_modelo;
    return logprob;      
}

float logprior(float *c, int poly_degree){
    return 0.0;
}

void MCMC_polynomial(float *x_obs, float *y_obs, int n_points, int n_steps, int poly_degree){
    srand48(10);
    float * c;
    float *l_param;
    float sigma = 0.1;
    float prop,logn,logv;
    float r,alfa;
    int i,j;
    c = new float[poly_degree+1];
    l_param = new *float [n_steps];
    for(i=0;i<poly_degree+1;i++){
        c[i] = drand48();
    }
    l_param[0] = &c;
    
    for (j=0; j < n_steps; j++){
       prop = l_param[j] + (2.0*drand48() - 0.5)*sigma;
       logn = loglikelikood(x_obs,y_obs,n_points,prop,poly_degree)+logprior(prop,poly_degree);
       logv = loglikelikood(x_obs,y_obs,n_points,l_param[j],poly_degree)+logprior(l_param[j],poly_degree);
                   
       r = exp(logn-logv);
       if(r > 1){
           r=1;
       }
       alfa = drand48();
       if(alfa < r){
           l_param[j+1] = prop;                   
       }
       else{
           l_param[j+1] = l_param[j];
       }
    }
        
    cout << l_param [0] << l_param[1] << l_param [2] << l_param[3] << endl;
            
    
}

float *read_file(string filename, int *n_points){
  int n_lines=0;
  ifstream infile; 
  string line;
  int i;
  float *array;

  infile.open(filename); 
  getline(infile, line);
  while(infile){
    n_lines++;
    getline(infile, line);
  }
  infile.close();
  *n_points = n_lines;
  array = new float[n_lines];


  i=0;
  infile.open(filename); 
  getline(infile, line);  
  while(infile){
    array[i] = atof(line.c_str());
    i++;
    getline(infile, line);
  }
  infile.close();

  return array;
}
