# ////////////////////////////////////////////////////////////////////////////////////////////////
# Adapted for the FNAL MMBC Project by I. Vohra (ivohra@exeter.edu, ishaanklv@gmail.com)
# ////////////////////////////////////////////////////////////////////////////////////////////////


# for shiftz in 57400 50225 43050 35875 28700 21525 14350 7175 0.1; do
#     for shiftx in 0.1 775 1550 2325 3100 3875 4650 5425 6200; do
#         for shifty in 0.1 775 1550 2325 3100 3875 4650 5425 6200; do

#             NEAR_DET_X_IN_CM=$shiftx;
#             NEAR_DET_Y_IN_CM=$shifty;
#             NEAR_DET_Z_IN_CM=$shiftz;

#             echo "Calculating flux for position ($NEAR_DET_X_IN_CM,$NEAR_DET_Y_IN_CM,$NEAR_DET_Z_IN_CM)"

#             # CD1 CDR reference beam w/ 120 GeV protons and 200 kA horn currents
#             root -q -b makeFluxHistograms.C\(\"$PWD\",\"100000\",\"$NEAR_DET_X_IN_CM\",\"$NEAR_DET_Y_IN_CM\",\"$NEAR_DET_Z_IN_CM\"\,\""OffAxis_${shiftx}_${shifty}_${shiftz}"\"\);

#         done
#     done
# done


for shiftz in 1 2 3; do
    for shiftx in 1 2 3; do
        for shifty in 1 2 3; do

            NEAR_DET_X_IN_CM=$shiftx;
            NEAR_DET_Y_IN_CM=$shifty;
            NEAR_DET_Z_IN_CM=$shiftz;

            echo "Calculating flux for position ($NEAR_DET_X_IN_CM,$NEAR_DET_Y_IN_CM,$NEAR_DET_Z_IN_CM)"

            # CD1 CDR reference beam w/ 120 GeV protons and 200 kA horn currents
            root -q -b makeFluxHistograms.C\(\"$PWD\",\"100000\",\"$NEAR_DET_X_IN_CM\",\"$NEAR_DET_Y_IN_CM\",\"$NEAR_DET_Z_IN_CM\"\,\""OffAxis_${shiftx}_${shifty}_${shiftz}"\"\);

        done
    done
done

#////////////

# # Define the min, max, and increment values for each axis
# init_x=1
# final_x=6200
# increment_x=1000

# init_y=1
# final_y=6200
# increment_y=1000

# init_z=57400
# final_z=1
# increment_z=-1000 #z increment is negative

# # Loop through the values
# for shiftz in $(seq $init_z $increment_z $final_z); do
#     for shiftx in $(seq $init_x $increment_x $final_x); do
#         for shifty in $(seq $init_y $increment_y $final_y); do

#             NEAR_DET_X_IN_CM=$shiftx
#             NEAR_DET_Y_IN_CM=$shifty
#             NEAR_DET_Z_IN_CM=$shiftz

#             echo "Calculating flux for position ($NEAR_DET_X_IN_CM,$NEAR_DET_Y_IN_CM,$NEAR_DET_Z_IN_CM)"

#             # CD1 CDR reference beam w/ 120 GeV protons and 200 kA horn currents
#             root -q -b makeFluxHistograms.C\(\"$PWD\",\"100000\",\"$NEAR_DET_X_IN_CM\",\"$NEAR_DET_Y_IN_CM\",\"$NEAR_DET_Z_IN_CM\"\,\""OffAxis_${shiftx}_${shifty}_${shiftz}"\"\)
#         done
#     done
# done
