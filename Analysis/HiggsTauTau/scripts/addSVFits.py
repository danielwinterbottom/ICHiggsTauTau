import uproot
import numpy as np
import pandas as pd
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True  # disable ROOT internal argument parser
import logging
from array import array
import argparse
import glob

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
    # First check if multiple svfit output files
    svfit_files = glob.glob(
        "{}/svfit_{}_{}_{}_*_output.root".format(
            args.svfit_path, args.intree, args.channel, args.year
    ))

    # If there are then proceed with looping over all
    if len(svfit_files) > 0:
        df = pd.DataFrame()
        dfs = []
        for index, svfit_file in enumerate(svfit_files):
            f = uproot.open(svfit_file)["svfit"]
            df_tmp = f.pandas.df(["event","run","lumi","svfit_mass","svfit_mass_err"],
                namedecode="utf-8").set_index(["event","run","lumi"])
            dfs.append(df_tmp)
        df = pd.concat(dfs)

        # Make sure df shape is non-zero
        assert df.shape[0] is not 0

    else:
        f = uproot.open(
            "{}/svfit_{}_{}_{}_output.root".format(
                args.svfit_path, args.intree, args.channel, args.year
        ))["svfit"]
        df = f.pandas.df(["event","run","lumi","svfit_mass","svfit_mass_err"],
            namedecode="utf-8").set_index(["event","run","lumi"])

        # Make sure df shape is non-zero
        assert df.shape[0] is not 0

    # Use these two dictionaries to retrieve mass and error on mass given 
    # event, run, lumi numbers (indices)
    mass_dict = df.to_dict()["svfit_mass"]
    mass_err_dict  = df.to_dict()["svfit_mass_err"]

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

    # Check number of events is same in svfit file and ntuple file
    assert tree.GetEntries() == df.shape[0]

    # Branches to write out to file_
    outmass = array("d", [-999])
    outbranch = tree.Branch(args.tag, outmass, "{}/D".format(args.tag))

    outmass_err = array("d", [-999])
    outbranch_err = tree.Branch(
        "{}_err".format(args.tag), outmass_err, "{}_err/D".format(args.tag)
    )

    mass = None
    mass_err = None

    for i_event in range(tree.GetEntries()):
        tree.GetEntry(i_event)

        event = int(getattr(tree, "event"))
        run   = int(getattr(tree, "run"))
        lumi  = int(getattr(tree, "lumi"))

        # print(event, lumi, run)

        outmass[0]     = -9999.
        outmass_err[0] = -9999.
        try:
            mass           = mass_dict.pop((event, run, lumi))
            outmass[0]     = mass
            mass_err       = mass_err_dict.pop((event, run, lumi))
            outmass_err[0] = mass_err
        except KeyError: 
            outmass[0]     = -9999.
            outmass_err[0] = -9999.

        if i_event % 10000 == 0:
            logger.debug('Currently on event {}'.format(i_event))

        outbranch.Fill()
        outbranch_err.Fill()

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
