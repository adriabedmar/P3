#!/bin/bash

# Be sure that this file has execution permissions:
# Use the nautilus explorer or chmod +x optimization.sh

# Write here the name and path of your program and database
DIR_P3=$HOME/PAV/P3
DB=$DIR_P3/pitch_db/train/
CMD=$DIR_P3/build/release/src/get_pitch/get_pitch

#arrays for threshold and time values
P=($(seq 42.5 0.5 43))
R1=($(seq 0.9 0.01 0.95))
RM=($(seq 0.45 0.01 0.5))
C=($(seq 0.015 0.005 0.02))

for p in "${P[@]}"; do
    for r1 in "${R1[@]}"; do
        for rm in "${RM[@]}"; do
            for cl in "${C[@]}"; do
                echo "P = $p , R1 = $r1 , RM = $rm , C = $cl"
                for filewav in $DB/*.wav; do

                    filef0=${filewav/.wav/.f0}
                    $CMD $filewav $filef0 -p $p -r $r1 -m $rm -c $cl || exit 1
                    #$CMD $filewav $filef0 -p $p -r $r1 -m $rm || exit 1

                done
                $DIR_P3/build/release/src/get_pitch/pitch_evaluate $DB/*.f0ref
                #Optimization for rl002.wav
                # $CMD $DIR_P3/rl002.wav $DIR_P3/rl002.f0 -p $p -r $r1 -m $rm || exit 1
                # $DIR_P3/build/release/src/get_pitch/pitch_evaluate $DIR_P3/rl002.f0ref
            done
        done
    done
done

exit 0