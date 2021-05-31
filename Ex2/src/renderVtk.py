import pyvista as pv
import numpy as np
import os
import platform
import inspect
import argparse
import matplotlib.pyplot as plt
import argparse

from common import *

print("Launching rendserVtk.py")
print("parsing arguments")
parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('infile', metavar='f', type=str)
parser.add_argument('outfile', metavar='o', type=str)

parser.add_argument('--display', dest='display', action='store_true')
parser.add_argument('--no-display', dest='display', action='store_false')
parser.set_defaults(display=True)

args = parser.parse_args()

# Load File
print(f"parsing {args.infile}")
mesh = pv.read(args.infile)

print(args.display)
# Render it!
print(f"saving rendered image to {args.outfile}")
mesh.plot(
    show_edges=False, 
    show_grid=True, 
    notebook=False, 
    off_screen=(not args.display), 
    screenshot=args.outfile
)
