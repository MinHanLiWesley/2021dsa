import multiprocessing as mp



# a = [1,2,3]
# b = [4,5,6]
# lst=[]
# for x,y in zip(a,b):
#     print(f"I am {x}")
#     lst.append(x**2 + y )

# print(lst)














def foo(x,y):
    print(f"I am {x}")
    return x**2 +y

bar_lst=[(1,4),(2,5),(3,6)]

with mp.Pool(2) as pool:
    a = pool.starmap(foo,bar_lst)
    
    
lst = a
print(lst)