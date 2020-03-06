import uproot
import numpy as np
import pandas as pd
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True  # disable ROOT internal argument parser
import logging
from array import array
import argparse

logger = logging.getLogger("addSVFits.py")
logger.setLevel(logging.DEBUG)
formatter = logging.Formatter("%(name)s - %(levelname)s - %(message)s")
handler = logging.StreamHandler()
handler.setFormatter(formatter)
logger.addHandler(handler)

def parse_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument("--path", default="./output/",
            help="Where are the trees to be annotated stored?")
    parser.add_argument("--svfit_path", default="./output/",
            help="Where are the svfit tree to be used?")
    parser.add_argument("--intree",
            help="File name to be added to.")
    parser.add_argument("--tag", default="test",
            help="Prefix to branch name: <tag>_mass")
    parser.add_argument("--channel", default="tt",
            help="What channel to run for")
    parser.add_argument("--year", default="2016",
            help="What year to run for")

    return parser.parse_args()

def load_files(filelist):
    with open(filelist) as f:
        file_names = f.read().splitlines()

    return file_names

def main(args):

    # Open file with svfits/fastMTTs
    f = uproot.open(
        "{}/svfit_{}_{}_{}_output.root".format(
            args.svfit_path, args.intree, args.channel, args.year
            )
        )["svfit"]
    df = f.pandas.df(["event","run","lumi","svfit_mass"],
        namedecode="utf-8").set_index(["event","run","lumi"])
    d = df.to_dict()["svfit_mass"]

    # Open file to annotate
    file_ = ROOT.TFile.Open(
        "{}/{}_{}_{}.root".format(
            args.path, args.intree, args.channel, args.year
            ), 
        "UPDATE")
    if file_ == None:
        logger.fatal("File %s is not existent.", args.intree)
        raise Exception
    tree = file_.Get("ntuple")


    outmass = array("d", [-999])
    outbranch = tree.Branch(args.tag, outmass, "{}/D".format(args.tag))

    mass = None

    for i_event in range(tree.GetEntries()):
        tree.GetEntry(i_event)

        event = int(getattr(tree, "event"))
        run = int(getattr(tree, "run"))
        lumi = int(getattr(tree, "lumi"))

        # print(event, lumi, run)

        outmass[0] = -999.
        try:
            mass = d.pop((event, run, lumi))
            outmass[0] = mass
        except KeyError: outmass[0] = -999.

        if i_event % 10000 == 0:
            logger.debug('Currently on event {}'.format(i_event))

        outbranch.Fill()

    logger.debug("Finished looping over events")

    # Write everything to file
    file_.Write("ntuple",ROOT.TObject.kWriteDelete)
    file_.Close()

    logger.debug("Closed file")

if __name__ == "__main__":
    args = parse_arguments()
    # file_names = load_files(args.intree)
    # main(args, file_names)
    main(args)
