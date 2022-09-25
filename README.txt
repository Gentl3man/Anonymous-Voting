Author: Giorgos Dovas

Generalises what is happening with secret_sharing
2 options:
    random <votes>:  creates a votes degree polynomial and solves it for x from 1 to votes
                     and stores the point to a file anon_votes.txt

    solve <file_name> : The file must be in the correct format or else it cant be sovled
                        Takes the num of votes and the points and prints out the sum of
                        positive votes.

                        Although there is a problem with big numbers, it doesnt always show the correct
                        result, i assume its because of bits "wraping-around" but up to 10-12 points runs
                        fine (at least on my machine)

To test it just run the bash script