#!/usr/bin/env python3.8

# May use math/import specific packages to speed up
import subprocess
import random
import math
import statistics
import matplotlib.pyplot as plt
import datetime
import numpy as np
subprocess.call('clear',shell=True)
def solve_tsp(filename, *args, **kwargs):
    '''
    Quick and dirty solver, not using classes and objects.
    Uses simulated annealing to stochastically calculate optimum route for a set
    of cities by swapping positions of the cities and trialing if the distance
    is lowered.
    '''
    with open(filename,'r') as infile:
        cities = [[float(x_y) for x_y in li.split(" ")] for li in infile]
        
    city_order, d = optimise_route(cities, *args, **kwargs)
    return city_order, min(d), d
    

def simulated_annealing(d_old, d_new, *args, beta, **kwargs):
    # Walrus operator - new in python3.8
    if (dr:= d_new - d_old) < 0:
        # Do first check to save computation of the log term. Auto accept
        # good guesses
        return True
    elif -beta * dr >= np.log(random.random()):
        # Log term prevents overflow from giant exponent (value
        # range is [-infty,0))
        return True
    else:
        # Bad guess, not close enough to accept.
        return False

def dr(r_1, r_2):
    return ((r_1[0] - r_2[0])**2 + (r_1[1]-r_2[1])**2)**(0.5)

def get_d(c_l, l_t):
    # Calculate journey distance from list order
    d = 0
    for i in range(0, len(c_l)-1):
        d+= l_t[c_l[i],c_l[i+1]]
    # Add distance to return to start
    d+= l_t[c_l[0],c_l[-1]]
    return d

def swap_cities(c_l, i, j):
    # Swap two random cities from the list
    t = c_l[i],c_l[j]
    c_l[j],c_l[i] = t
    return c_l

def gen_lookup(c_l):
    # Generate lookup tables so all calcs are done at init
    l_t   = np.empty([int(len(c_l)), int(len(c_l))], dtype = object)
    c_l_n = list()
    for i in range(0,len(c_l)):
        c_l_n.append([i,c_l[i]])
        for j in range(i+1, len(c_l)):
            l_t[i,j] = dr(c_l[i],c_l[j])
            l_t[j,i] = dr(c_l[i],c_l[j])
    return c_l_n, l_t

def get_b0(c_l, l_t, p_0,  **kwargs):
    # Get initial simulation temperature from init proabbility
    p = list()
    for i in range(0,int(len(c_l)**2)):
        p.append(l_t[tuple(random.sample(c_l, k=2))])
    return -np.log(p_0) /  np.mean(p)

def optimise_route(r_l, *args,
                   n_bet,f_max, b_x,f_loc, n_t,**kwargs):
    j = 1
    d_l = list() # list of distances
    r_l, l_tab = gen_lookup(r_l)
    c_l = list(range(0,len(r_l)))
    while True:
        print(f'Run {j}')
        fail, f_o, loops, t  = 0, 0, 0, 0 # Initialise looping vars
        random.shuffle(c_l)
        beta  = get_b0(c_l, l_tab, **kwargs) # initial beta off of p_0
        d     = None
        b_l   = c_l.copy()
        print(f'Beta = {beta:5.05f}',end="\r")
        while loops <= n_bet:
            c_l =  swap_cities(c_l, *random.sample(c_l, k = 2))
            t   = get_d(c_l, l_tab)
            if not d:
                d = t
            if simulated_annealing(d, t, beta = beta, **kwargs):
                if t < d:
                    d, fail, f_o = t, 0, 0
                    b_l = [c_i for c_i in c_l]
                else:
                    f_o  += 1
                    fail += 1
            else:
                fail += 1
                f_o  += 1
            if f_o > f_loc:
                c_l = [c_i for c_i in b_l]
                f_o = 0
            elif fail > f_max:
                beta   *= b_x
                print(f'Beta = {beta:5.05f}',end="\r")
                fail,f_o = 0, 0
                loops += 1
                c_l = [c_i for c_i in b_l]
                i = 0
        if not d_l or d < min(d_l):
            t_l = [c_i for c_i in b_l]
        j += 1
        print(f'\nd = {d:.05f}')
        i_0 = 0
        # random stoch optimiser
        while True:
            i_0 += 1
            c_l =  swap_cities(c_l, *random.sample(c_l, k = 2))
            if (t:= get_d(c_l, l_tab)) < d:
                d = t
                t_l = [c_i for c_i in c_l]
            else:
                c_l = [c_i for c_i in t_l]
            if i_0 > f_max:
                break
        d_l.append(d)
        if j > n_t:
            return t_l, d_l

inputs = {'p_0'   : 0.9,  # Initial probability used for beta
          'b_x'   : 1.1,  # Beta scale with every outer loop
          'f_max' : 10000, # How many fails before beta reduced
          'f_loc' : 500,  # Fail counter for return to globmin
          'n_bet' : 25,   # Number of beta multiplies
          'n_t'   : 3}    # Total number of iterations of script
cities, d, d_l = solve_tsp('cities.txt', **inputs)

print(f'Total distance = {d:.03f}')
print(f'City              Coordinates')
for c_n, c in enumerate(cities):
    print(f'{c_n+1:<7} {c}')
cities.append(cities[0])
plt.plot(*zip(*cities))
plt.grid(True)
t = datetime.datetime.now().strftime("%Y-%m-%d-%H-%M-%S")
plt.savefig(f'travelling_salesman.pdf')
plt.figure(123)
n,bins, _ =plt.hist(d_l, 30)
plt.figure(2)
bin_width = bins[1] - bins[0]
print(n,bins)
plt.plot([b + bin_width for b in bins[:-1]], n, 'o')
plt.savefig(f'{t}-dist_of_guesses.pdf')
