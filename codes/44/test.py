import numpy as np
import random

N = 20
M =2
O = M+N
L = 5
inline = 0
package_table = [*range(1, N+1)]
line_table = [*range(L)]

g_pop_count = N
g_push_count = N
g_merge_count = M

pop_array = []


def main():

    global lineset
    global O
    global line_table
    global inline
    lineset = []
    for i in range(L):
        lineset.append([])

    pop_list = [pop_first, pop_last, pop_max]

    while(O != 0):
        if package_table:
            O -= 1
            a = random.choice(package_table)
            b = random.choice(line_table)
            push(a, b)
            inline += 1
        while random.random() > 0.6 and g_pop_count > 2 and inline != 0:
            while(1):
                l = random.choice(line_table)
                if not lineset[l]:
                    continue
                else:
                    random.choice(pop_list)(l)
                    inline -=1
                    break
        while random.random() > 0.7 and g_merge_count > 0:
            l1, l2 = random.sample(line_table, 2)
            merge(l1, l2)
            O -= 1
    while (g_push_count == 0 and g_pop_count != 0):
        while(1):
            l = random.choice(line_table)
            if not lineset[l]:
                continue
            else:
                random.choice(pop_list)(l)
                break


def push(a, b):
    global g_push_count
    global lineset
    global package_table
    lineset[b].append(a)
    package_table.remove(a)
    g_push_count -= 1
    print(f"push {a} {b}")


def pop_first(l):
    global g_pop_count
    global lineset
    global pop_array
    pop_array.append(lineset[l][0])
    lineset[l].remove(lineset[l][0])
    g_pop_count -= 1


def pop_last(l):
    global g_pop_count
    global lineset
    global pop_array
    pop_array.append(lineset[l][-1])
    lineset[l].remove(lineset[l][-1])
    g_pop_count -= 1


def pop_max(l):
    global g_pop_count
    global lineset
    global pop_array
    pop_array.append(max(lineset[l]))
    lineset[l].remove(max(lineset[l]))
    g_pop_count -= 1


def merge(a, b):
    global lineset
    global line_table
    global g_merge_count
    lineset[b] = lineset[b] + lineset[a]
    lineset[a] = []
    line_table.remove(a)
    g_merge_count -=1
    print(f"merge {a} {b}")


if __name__ == '__main__':
    T= 1
    print(T)
    print(f"{N} {O} {L}")
    main()
    print(' '.join(str(i) for i in pop_array))
