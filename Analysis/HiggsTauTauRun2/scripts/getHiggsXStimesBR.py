import json
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--mass',help= 'Mass of Higgs boson', default='125.4')
args = parser.parse_args()

xs_json_name = 'input/YR4/Higgs_XS.json'
with open(xs_json_name) as json_file:
 xs_dict = json.load(json_file)

br_json_name = 'input/YR4/Higgs_XS.json'
with open(br_json_name) as json_file:
 br_dict = json.load(json_file)

print "ggH->tautau: ", xs_dict["ggH"]*br_dict[""]
