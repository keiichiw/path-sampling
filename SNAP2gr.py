#!/usr/bin/python
import sys
import os.path

def graph_info(infile):
    v = 0
    e = 0
    with open(infile, "r") as f:
        for row in f:
            if row[0] == "#":
                continue
            else:
                l = row.split()
                if len(l) != 2:
                    print "error ", l
                    exit(1)
                v = max(v, int(l[0]), int(l[1]))
                e += 1
    return (v + 1, e)

def main():
    argvs = sys.argv
    if len(argvs) != 2:
        print >> sys.stderr, "convert SNAP format to DIMACS format\nUsage: ./SNAP2gr.py <filename>"
        exit(1)
    infile = argvs[1]
    fname, ext = os.path.splitext(infile)
    outfile = fname + ".gr"
    (v, e) = graph_info(infile)
    print (v, e)
    with open(infile, "r") as ifile:
        with open(outfile, "w") as ofile:
            print >> ofile, "p sp ", v, " ", e
            for row in ifile:
                row = row.rstrip()
                if row[0] == "#":
                    print >> ofile, "c", row[1:]
                else:
                    print >> ofile, "a ", row, " 1"


if __name__ == "__main__":
    main()
