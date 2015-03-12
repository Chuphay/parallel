import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial.kdtree import KDTree 

mpl.rcParams['legend.fontsize'] = 10

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.set_zlim(-10, 10)

def make_gauss(n, mean=0, sd=1):
    x = np.random.randn(n)
    y = np.random.randn(n)
    z = np.random.randn(n)
    return np.array([x,y,z]).T


def make_ring(n, radius = 10, noise = 1):
    theta = 2*np.pi*np.random.rand(n)
    x = radius*np.cos(theta)+noise*np.random.randn(n)
    y = radius*np.sin(theta)+noise*np.random.randn(n)
    z = np.random.randn(n)*noise/10  #felt better
    return np.array([x,y,z]).T 

def plot_data(data, c = 'k', mark = '.'):
    ax.scatter(data[:,0],data[:,1],data[:,2], color = c, marker = mark)
    plt.show()

def make_saturn(n, sd = 1, r= 10, noise = 1):
    flag = n%2
    d_1 = make_ring(n//2, r, noise)
    d_2 = make_gauss(n//2+flag, sd = sd)
    return np.vstack((d_1,d_2))


 
def make_adjacency_matrix(data, k):
    kt = KDTree(data) 
    out = zeros((len(data),len(data)))
    for i, points in enumerate(data):
        distance, neighbors = kt.query(points, k+1)
        for j in neighbors[1:]:
            out[i][j] = 1;
            out[j][i] = 1; #to ensure symmetry, one can imagine the the nearest neighbors is not necessarily associative
    return out 

def make_edge_graph(data, k):
    kt = KDTree(data)
    out = [set() for i in data]
    k_out = k
    for i, points in enumerate(data):
        distance, neighbors = kt.query(points, k+1)
        for j in neighbors[1:]:
            out[i].add(j)
            if(len(out[i])>k_out):
                k_out = len(out[i])
            out[j].add(i)
            if(len(out[j])>k_out):
                k_out = len(out[j])
    return (k_out, out)

def write_data(edge_graph, k, file_name):
    N = len(edge_graph)
    f = open(file_name, 'w')
    f.write("{0} {1}\n".format(N,k))
    for i, d in enumerate(edge_graph):
        f.write("{0}".format(str(i)))
        for j in d:
            f.write(" {0}".format(str(j)))
        f.write("\n")
    f.close()
    
  
if __name__ == "__main__":
    num = 2000
    k = 15
    np.random.seed(123)
    g = make_saturn(num)
    k,x = make_edge_graph(g, k)
    print(k)
    k_avg = 0
    for d in x:
        k_avg = k_avg + len(d)
    print(k_avg/num)    
    write_data(x, k, "test.data")
    #plot_data(g)

