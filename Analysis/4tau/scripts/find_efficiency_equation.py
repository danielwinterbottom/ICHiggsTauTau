import itertools

#n = 4 # number of overlapping regions
#replace_list = ["trigeff_singlee_data_1","trigeff_doubletau_data_2*trigeff_doubletau_data_3","trigeff_doubletau_data_2*trigeff_doubletau_data_4","trigeff_doubletau_data_3*trigeff_doubletau_data_4"]
#replace_list_mc = ["trigeff_singlee_mc_1","trigeff_doubletau_mc_2*trigeff_doubletau_mc_3","trigeff_doubletau_mc_2*trigeff_doubletau_mc_4","trigeff_doubletau_mc_3*trigeff_doubletau_mc_4"]
#replace_list = [
#                "(d1*d2)",
#                "(d1*d3)",
#                "(d1*d4)",
#                "(d2*d3)",
#                "(d2*d4)",
#                "(d3*d4)",
#                ]

#replace_list = [
#                "(d1*d2)",
#                "(d1*d3)",
#                "(d2*d3)",
#                ]

#replace_list = [
#                "d1",
#                "(d2*d3)",
#                "(d2*d4)",
#                "(d3*d4)",
#                ]

#replace_list = [
#                "d1",
#                "d2",
#                "(d3*d4)",
#                ]


replace_list = [
                "d1",
                "d2",
                "d3",
                "d4"
                ]


replace_list_mc = [x.replace("d","m") for x in replace_list]
n = len(replace_list)
assume_times = True
do_SF = True
lst_n = range(1,n+1)

total = []
total_keys = []
for ind,i in enumerate(lst_n):
  for k in list(itertools.combinations(lst_n, ind+1)):
    total.append([k,1])
    total_keys.append(k)

for ind, val in enumerate(total):
  changes = []
  includes_all = True
  for j in total_keys:
    includes_all = True
    for i in val[0]:
      if not (i in j and len(val[0])<len(j)):
        includes_all = False
    if includes_all: 
      changes.append([j, -1])
  for ind_i,i in enumerate(changes):
    for ind_j,j in enumerate(total):
      if i[0] == j[0]:
        total[ind_j][1] += (total[ind][1]*changes[ind_i][1])

#print total

sum_string = "eff_total = "
for ind_i, i in enumerate(total):

  if i[1] < 0:
    sum_string = sum_string[:-2] + "- "
  elif i[1] == 0:
    continue

  if abs(i[1]) != 1:
    sum_string += str(i[1])

  sum_string += "eff("
  for ind_j,j in enumerate(i[0]):
    sum_string += str(j)
    if ind_j != len(i[0])-1:
      sum_string+= "+"
  sum_string+=")"
  if ind_i != len(total)-1:
    sum_string += " + "

print sum_string
print " "

if assume_times:
  sum_string = "SF_total = (("
  for ind_i, i in enumerate(total):
    if i[1] < 0:
      sum_string = sum_string[:-2] + "- "
    elif i[1] == 0:
      continue

    if abs(i[1]) != 1:
      sum_string += str(i[1])

    if len(i[0]) > 1:
      sum_string += "("
      for ind_j,j in enumerate(i[0]):
        sum_string += replace_list[j-1]
        if ind_j < len(i[0])-1: 
          sum_string += "*"
      sum_string += ")"
    else:
      sum_string += replace_list[i[0][0]-1]

    if ind_i != len(total)-1:
      sum_string += " + "

  sum_string += ") / ("

  for ind_i, i in enumerate(total):
    if i[1] < 0:
      sum_string = sum_string[:-2] + "- "
    elif i[1] == 0:
      continue

    if abs(i[1]) != 1:
      sum_string += str(i[1])

    if len(i[0]) > 1:
      sum_string += "("
      for ind_j,j in enumerate(i[0]):
        sum_string += replace_list_mc[j-1]
        if ind_j < len(i[0])-1:
          sum_string += "*"
      sum_string += ")"
    else:
      sum_string += replace_list_mc[i[0][0]-1]

    if ind_i != len(total)-1:
      sum_string += " + "
  sum_string += "))"


print sum_string

    


