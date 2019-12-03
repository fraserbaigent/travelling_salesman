#!/usr/bin/env python3.8

# May use math/import specific packages to speed up
import random
import math
import matplotlib.pyplot as plt
def quick_and_dirty(filename, *args, **kwargs):
    ''' Quick and dirty solver, not using nice classes and objects'''
    with open(filename,'r') as infile:
        cities = [[float(x_y) for x_y in li.split(" ")] for li in infile]
        
    city_order, d = optimise_route(cities, *args, **kwargs)
    return city_order, d
    

def simulated_annealing(d_old, d_new, *args, beta, **kwargs):
    if -beta * (d_new - d_old) > math.log(random.random()):
        return True
    else:
        return False
    
def get_distance(city_list):
    d = 0
    dr = lambda r_1, r_2: ((r_1[0] - r_2[0])**2 + (r_1[1]-r_2[1])**2)**(0.5)
    for i in range(0,len(city_list[:-1])):
        d+= dr(city_list[i], city_list[i+1])
    d+= dr(city_list[0],city_list[-1])
    return d

def get_new_order(city_list):
    p = [city_list.index(v) for v in random.choices(city_list,k=2)]
    city_list[p[0]],city_list[p[1]] =  city_list[p[1]],city_list[p[0]]
    return city_list

def optimise_route(city_list, *args,
                   thats_enough,fail_threshold, beta_scale, beta_0, **kwargs):
    fail = 0 # Fail counter
    t    = 0 # Total distance
    beta = beta_0
    d    = None
    best_list = city_list.copy()
    stages = 0
    while True:
        city_list = get_new_order(city_list)
        t = get_distance(city_list)
        if not d:
            d = t
        if simulated_annealing(d, t, beta = beta, **kwargs):
            if t < d:
                d    = t
                fail = 0
                best_list = [c_i for c_i in city_list]
            else:
                fail += 1
        else:
            fail += 1
        if fail > fail_threshold:
            if (stages <= thats_enough):
                print('making beta smaller')
                beta   *= beta_scale
                stages += 1
                fail    = 0
                city_list = [c_i for c_i in best_list]
            else:
                break
    return best_list, d

inputs = {'beta_0' :1.0,
          'beta_scale' : 0.5,
          'fail_threshold' : 1000000,
          'thats_enough' : 10}
cities, d = quick_and_dirty('cities.txt', **inputs)

print(cities, d)
plt.plot(*zip(*cities))
plt.savefig('test.pdf')
