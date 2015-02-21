import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm

data = list(map(float, open("data.txt").read().split()))
data = np.array(data).reshape(len(data)/2,2)

clusters = list(map(int, open("classes.txt").read().split()))
num_clusters = max(clusters)
colors = cm.rainbow(np.linspace(0, 1, num_clusters))

plt.figure(figsize = (15,3))
for i in range(num_clusters):
    for j in range(len(clusters)):
        if(clusters[j] == i + 1): #plus 1 because my clusters start at 1
            plt.scatter(data[j][0],data[j][1], c = colors[i])

plt.show();
