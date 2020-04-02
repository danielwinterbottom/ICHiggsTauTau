import subprocess

def transfer_to_dcache():
    with open("files_to_transfer_HToWW.txt", "r") as f:
        files = f.read().splitlines()
        file_names = [
            file_.split("/")[-2:] for file_ in files if ".root" in file_
        ]

    command = (
        'uberftp sedsk53.grid.hep.ph.ic.ac.uk "put -r '
        '/vols/cms/akd116/Offline/output/SM/2020/Mar02_2017_SVFit/{0}/{1} '
        'store/user/adow/Mar02_2017_SVFit/{0}/{1}" '
    )

    current_dir = ""
    current_file = ""
    for name in file_names:
        if "UP" in name[0] or "DOWN" in name[0]:
            # print(command.format(name[0], name[1]))
            subprocess.call(command.format(name[0], name[1]), shell=True)
        else:
            # print(command.format("", name[1]))
            subprocess.call(command.format("", name[1]), shell=True)


if __name__ == "__main__":
    transfer_to_dcache()
