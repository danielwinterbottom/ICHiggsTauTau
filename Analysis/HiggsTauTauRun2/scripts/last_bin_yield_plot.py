import matplotlib.pyplot as plt
import numpy as np

plt.rcParams.update({'font.size': 16})

# Data for plotting
gU = np.arange(0.0, 1.0, 0.01)

non_inter = 0.5906354
inter = -0.130620418

s1 = non_inter * (gU**4)
s2 = inter * (gU**2)
s3 = s1+s2

fig, ax = plt.subplots()
ax.plot(gU, s1, label='Non-interference')
ax.plot(gU, s2, label='Interference')
ax.plot(gU, s3, label='Combined')
ax.legend(loc=2)

ax.set(xlabel='$g_{U}$', ylabel='Signal Yield $m_{T}^{tot}$>900 GeV',
       title='')
ax.grid()

fig.savefig("test.png")
plt.show()

