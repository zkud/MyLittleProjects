"""
creates dia diagrams from source code
in current directory.
Autodia (http://www.aarontrevena.co.uk/opensource/autodia/)
"adapter" for people, who hates documentations.
depencies:
    * autodia (and it's depencies)
    * python3
    * sys
    * os
usage:
    python3 diagrams.py <language>
author:
    L1ttl3S1st3r
date:
    12/18/2018
"""
import sys
import os


def main():
    os.system(
        'autodia -l {} -d "." -a -m'.format(
            sys.argv[1]
        )
    )


if __name__ == '__main__':
    main()