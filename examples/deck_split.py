#!/usr/bin/env python
import sunbeam
import sys
import os


def output_deck(deck, output_path, input_name):
    sections = set(["RUNSPEC", "GRID", "PROPS", "REGIONS", "SCHEDULE" , "SUMMARY"])
    baseH = open( os.path.join( output_path , input_name), "w")
    fileH = baseH
    for kw in deck:
        if kw.name in sections:
            baseH.write("INCLUDE\n   \'%s\' /\n\n" % kw.name)
            fileH = open( os.path.join( output_path, kw.name ), "w")
        fileH.write( str(kw) )




if __name__ == "__main__":
    input_deck = sys.argv[1]
    output_path = sys.argv[2]

    deck = sunbeam.parse_deck( input_deck )
    if not os.path.isdir( output_path ):
        os.makedirs( output_path )

    output_deck( deck , output_path, os.path.basename( input_deck ))
