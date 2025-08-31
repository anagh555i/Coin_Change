def coin_change(coins, target):
    dp = [float('inf')] * (target + 1)
 
    dp[0] = 0

    for amount in range(1, target + 1):
        for coin in coins:
            if coin <= amount:
                dp[amount] = min(dp[amount], dp[amount - coin] + 1)

    return dp[target] if dp[target] != float('inf') else -1
def swap(h,i,j):
    h[i]=h[j]+h[i]
    h[j]=h[i]-h[j]
    h[i]=h[i]-h[j]
    return
def heapify(h,i):
    m=i
    if (2*i+1<len(h) and h[i]>h[2*i+1]):
        m=2*i+1
    if(2*i+2<len(h) and h[m]>h[2*i+2]):
        m=2*i+2
    if m!=i:
        swap(h,i,m)
        heapify(h,m)
    return

def hinsert(h,v):
    h.append(v)
    i=len(h)-1
    while(i!=0 and h[(i-1)//2]>h[i]):
        swap(h,i,(i-1)//2)
        i=(i-1)//2

def hpop(h):
    l=len(h)-1
    if l>0:
        swap(h,0,l)
    v=h.pop()
    if(l>1):
        heapify(h,0)
    return v

def bestFS(tgt,coins):
    open=[tgt]
    closed=[]
    d=dict()
    x=-1
    d[tgt]=x
    n=0
    #print(coins)
    while(len(open)>0):
        n+=1
        x=hpop(open)
        #print("x ",x)
        if x==0:
            break
        for i in coins:
            c=x-i
            if c>=0 and c not in open and c not in closed and i>0:
                d[c]=x
                #print("c d[c]",c,d[c])
                hinsert(open,c)
        closed.append(x)
    x=0
    moves=[]
    #print(x)
    #print(d[x])
    r=len(moves)
    if(0 not in d):
        r=-1
    while ( r!=-1 and d[x]!=-1 ):#and d[d[x]]!=-1):
        moves.append(d[x]-x)#d[d[x]]-d[x])
        x=d[x]
    dv=coin_change(coins,t)
    if(r!=-1):
        r=len(moves)
    if dv==r:
        s="Success"
    else :
        s="Fail"
    print(r,dv,n,s,sep="       ")
    return r

'''
coins = input()
coins= coins.split()
coins=[eval(i) for i in coins ]
t=int(input())
print(bestFS(t,coins))'''
inp=input()
a=0
n=0
print("****************************  BestFS ALGORITHM  ******************************")
print("**************************************************************************")
print("BestFS-Solution  DP-Solution    no of nodes      Status")
while(inp!='-1'):
    t=inp.split()
    t=eval(t[1])
    coins = input()
    coins= coins.split()
    coins=[eval(i) for i in coins ]  
    #print("for amount",t,"in coins",coins,":")    
    #print(n,": target",t,"coins",coins)
    v=bestFS(t,coins)
    dv=coin_change(coins,t)
    #print(v,dv)
    n+=1
    if(v==dv):
        a+=1
    inp=input()    
print("**************************************************************************")
print("**************************************************************************")
print("Success : ",a)
print("Fail    : ",n-a)
print("Accurracy : ",a/n)