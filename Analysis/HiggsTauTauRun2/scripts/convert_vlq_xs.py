from collections import OrderedDict

dict = {}
file = open('scripts/vlq_xs_from_gridpacks.txt', 'r')
for line in file:
  dict[line.split(" ")[0]] = float(line.split(" ")[1])

new_dict = {}

for k,v in dict.items():
  if "interference" not in k:
    new_dict[k + "_matched"] = v
  elif "interference" in k and "matched" in k:
    new_dict[k] = v * (dict[k.replace("highM","lowM").replace("_matched","")] + dict[k.replace("lowM","highM").replace("_matched","")])/(dict[k.replace("highM","lowM")] + dict[k.replace("lowM","highM")])

od = OrderedDict(sorted(new_dict.items()))

for k, v in od.iteritems(): print k,v
