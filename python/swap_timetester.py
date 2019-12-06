import timeit


case_1 = """
a = [[1,2],[2,3],[3,4],[4,5]]
p = [1,3]
e_1 = a.pop(p[0])
e_2 = a.pop(p[1]-1)
a.insert(p[0],e_2)
a.insert(p[1],e_1)

"""

case_3 = """
a = [[1,2],[2,3],[3,4],[4,5]]
p = [1,3]
a[p[0]],a[p[1]] = a[p[1]],a[p[0]]

"""

case_2 = """
a = [[1,2],[2,3],[3,4],[4,5]]
p = [1,3]
t = a[p[0]],a[p[1]]
a[p[1]],a[p[0]] = t
"""


for c in (case_1,case_2,case_3):
    print(timeit.timeit(c, number = 100000)/100000.)
