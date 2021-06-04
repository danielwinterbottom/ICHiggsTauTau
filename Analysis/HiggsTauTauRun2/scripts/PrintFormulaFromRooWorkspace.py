import ROOT
import argparse
import re
import ast

parser = argparse.ArgumentParser()
parser.add_argument('--input','-i', help= 'Name of input RooWorkspace', default='input/fake_factors/fakefactors_ws_mt_mssm_2018_v2.root')
parser.add_argument('--function','-f', help= 'Name of input function in RooWorkspace', default='ff_lt_wjets')
parser.add_argument('--replace','-r', help= 'Replace name dict', default=None)
args = parser.parse_args()

input = args.input
function = args.function
if args.replace != None: 
  replace = ast.literal_eval(args.replace)
else:
  replace = args.replace

def find_number(text,c):
  return re.findall(r'%s(\d+)' % c, text)

def GetFormula(func,total_func,RooFuncList,it_2):
  func_str = func.formula().GetExpFormula().Data()
  params = find_number(func_str,"@")

  # Get number of Roo functions already in string
  if find_number(total_func,"PAR") == []: num_par = 0
  else: num_par = max([int(i) for i in find_number(total_func,"PAR")])

  if params == []: max_par = 0
  else: max_par = max([int(i) for i in params])

  for i in range(0,max_par+1):
    func_str = func_str.replace('@'+str(i),"PAR"+str(len(RooFuncList))+'_')
    RooFuncList.append(func.getParameter(i))
  return func_str,RooFuncList

print "Getting Function for {}:{}".format(input,function)

f = ROOT.TFile(input)
w = f.Get("w")
RooFuncList = []
func = w.function(function)
it = 0
func,RooFuncList = GetFormula(func,func.formula().GetExpFormula().Data(),RooFuncList,0)
for i in RooFuncList:
  if str(type(i)) == "<class 'ROOT.RooFormulaVar'>":
    func_new,RooFuncList = GetFormula(i,func,RooFuncList,1)
    func = func.replace('PAR'+str(it)+'_','('+func_new+')')
    RooFuncList[it] = None
  it += 1

for i in range(0,len(RooFuncList)):
  if RooFuncList[i] != None:
    func = func.replace("PAR"+str(i)+'_',RooFuncList[i].GetName())

if replace != None:
  replace_dict = {}
  for c,v in replace.items():
    math_list=['(',')','+','-','*','/','<','>','=',',']
    for i in [m.start() for m in re.finditer(c, func)]:
      if func[i-1] in math_list and func[i+len(c)] in math_list:
        replace_dict[i] = c
  b = 0
  for k in sorted(replace_dict):
    func = func[:k+b] + replace[replace_dict[k]] + func[k+len(replace_dict[k])+b:]
    b += len(replace[replace_dict[k]]) - len(replace_dict[k])

print func
