#!/bin/bash
#just do the job for me

clear

green_text=`tput setaf 10`
black_background=`tput setab 0`
reset=`tput sgr0`
VOTES=10;
num_of_args=$#
usage()
{
    echo "${black_background}${green_text}Running anon_voting test"${reset} #i find it fancy 
    echo "This test takes a number as a parameter and creates random votes (points)."
    echo "The votes are inserted to a file named 'anon_votes' and that file is used to find the sum of votes."
    echo "If no number is given it chooses 10 votes by default."
    echo
    echo
}

#################################################
#                 MAIN PROGRAM                 #
#################################################

if [ `expr $num_of_args` == 0 ]
then
    usage
else
    VOTES="${1}"
fi
OUTPUT="$(./anon_voting random $VOTES)"
echo "${VOTES} VOTES have been created, and the points have been set"
echo "${OUTPUT}"

OUTPUT="$(./anon_voting solve anon_votes.txt)"
echo "${OUTPUT}"



echo "${black_background}${green_text}Secret_sharing FINISHED"${reset}
