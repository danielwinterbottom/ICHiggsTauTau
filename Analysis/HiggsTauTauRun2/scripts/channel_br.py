decay_br = {"t":0.648,"m":0.174,"e":0.178}

no_in_final_state = 4

col = []
for i in range(0,no_in_final_state):
  if i == 0:
    for key in decay_br.keys():
      col.append([key])
  else:
    prev_col = col[:]
    col = []
    for j in prev_col:
      for key in decay_br.keys():
        col.append(j+[key])

print col

for i,j in enumerate(col): col[i] = sorted(j)

comb = []
count = []

for i in col:
  if i not in comb:
    comb.append(i)
    count.append(1)
  else:
    count[comb.index(i)] += 1

br = []
for i,j in enumerate(comb):
  br.append(count[i])
  for k in j:
    br[i] *= decay_br[k]

list1, list2 = zip(*sorted(zip(br, comb)))

a = list(list2)
b = list(list1)

a.reverse()
b.reverse()

sum_bi = 0
for i,j in enumerate(a):
  print ''.join(j), "{}%".format(round(b[i]*100,1))
  sum_bi += b[i]

print sum_bi
