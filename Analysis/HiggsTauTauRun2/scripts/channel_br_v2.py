decay_br_1 = {"\\tau_h^{+}":0.648,"\\mu^{+}":0.174,"e^{+}":0.178}
decay_br_2 = {"\\tau_h^{-}":0.648,"\\mu^{-}":0.174,"e^{-}":0.178}
decay_br = {"\\tau_h^{-}":0.648,"\\mu^{-}":0.174,"e^{-}":0.178,"\\tau_h^{+}":0.648,"\\mu^{+}":0.174,"e^{+}":0.178}

col = []
one_dec = []
for i in decay_br_1:
  for j in decay_br_2:
    one_dec.append([i,j])

for i in one_dec:
  for j in one_dec:
    col.append(i+j)

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
  print "$"+''.join(j)+"$", "& {}\\% \\\\".format(round(b[i]*100,1))
  sum_bi += b[i]

#print "Total", "{}%".format(round(sum_bi*100,1))
