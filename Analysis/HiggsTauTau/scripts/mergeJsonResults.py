#!/usr/bin/env python
import argparse
import json
import os

parser = argparse.ArgumentParser()

parser.add_argument('--type', default='simple')
parser.add_argument('--output', default='merged.json')
parser.add_argument('input', nargs='+')

args = parser.parse_args()

results = {}


def run(cmd):
    print cmd
    os.system(cmd)

if args.type == 'triggerinfo':
    results['yields'] = {}
    for arg in args.input:
        with open(arg) as jsonfile:
            js = json.load(jsonfile)
            for trg in js['yields']:
                if trg not in results['yields']:
                    results['yields'][trg] = {}
                for run in js['yields'][trg]:
                    if run not in results['yields'][trg]:
                        results['yields'][trg][run] = 0
                    results['yields'][trg][run] += js['yields'][trg][run]

if args.type == 'lumi':
    run('cp %s %s' % (args.input[0], args.output))
    for arg in args.input[1:]:
        run('compareJSON.py --or %s %s %s' % (args.output, arg, 'tmp.json'))
        run('mv %s %s' % ('tmp.json', args.output))


if args.type in ['triggerinfo']:
    jsondata = json.dumps(results, sort_keys=True, indent=2, separators=(',', ': '))
    with open(args.output, 'w') as out_file:
        out_file.write(jsondata)
