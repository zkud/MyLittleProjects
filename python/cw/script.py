# !python3
"""
description: 
    Makes an rtf output of yours source files for course work
usage:
    python3 script.py <file extention template like "*.py">
depencies:
    glob, os, sys, pygments
author:
    L1ttl3S1st3r
date:
    12/16/2018
"""
import glob
import os
import sys


def main():
    # make an output directory
    try:
        os.mkdir('output')
    except FileExistsError:
        pass

    # go through the all source files in directory
    for source_path in glob.glob(sys.argv[1]):
        # make .rtf output of them
        os.system(
            'pygmentize -o {} {}'.format(
                'output' +  os.sep + source_path + '.rtf',
                source_path
            )
        )


if __name__ == "__main__":
    main()
