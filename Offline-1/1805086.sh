# At first list the files in the directory submissions
# Then, for each file in the directory, give the permission to execute
# Then, for each file in the directory, execute the file and save the output in a file
# Then, for each file in the directory, remove the file
# Then, compare the output with other files in the directory
# Then, for each file in the directory, remove the file


#echo students_id, score in a otuput file
echo "students_id, score" > output.csv

#take the arguments from the command line
#the first argument is the max score
#the second argument is the last character of the students_id

if [ $# -eq 2 ]; then
    max_score=$1
    last_char=$2
elif [ $# -eq 1 ]; then
    max_score=$1
    last_char=5
else
    max_score=100
    last_char=5
fi

files=$(ls Submissions)         # list the files in the directory submissions


for file_1 in $files; do        # for each file in the directory
    #if the file is greater than the specified last character of the students_id
    #then, continue to the next file (do not execute the file)
    if [ ${file_1: -1} -gt $last_char ]; then
        continue
    fi
    

    if [ -d Submissions/$file_1 ] && [ -f Submissions/$file_1/$file_1.sh ]; then         # if the file is a directory proceed
        echo $file_1
        chmod 554 Submissions/$file_1;                  # give the permission to execute permission to owner and group 
                                                        # and readpermission to all (folder)
        chmod 554 Submissions/$file_1/$file_1.sh;       # same permission to sh files....

        # execute the file and save the output in a file
        Submissions/$file_1/$file_1.sh > Submissions/$file_1.txt;

        # compare the output with the accepted output
        diff Submissions/$file_1.txt AcceptedOutput.txt --ignore-all-space > Submissions/$file_1-diff.txt;

        # count the number of lines in the diff file(this is the number of differences)
        val=$(grep -c "^[<>]" Submissions/$file_1-diff.txt);
        val=$(($val*5));        # for each difference, 5 points are deducted

        rm Submissions/$file_1.txt;                 # remove the output file
        rm Submissions/$file_1-diff.txt;            # remove the diff file
        
        score=$(($max_score-$val));                 # calculate the score

        if [ $score -lt 0 ]; then           # if the score is less than 0
            score=0;                        # set the score to 0
        fi

        #now copy checker inmlementation
        #compare the current student's file with all the other students' files
        #if there is a match, then the student gets -score
        for file_2 in $files; do
            if [ -d Submissions/$file_2 ] && [ -f Submissions/$file_2/$file_2.sh ]; then            # if the file is a directory and there is a sh file in the directory
                if [ "$file_1" != "$file_2" ]; then                                                 # if the file is not the current student's file

                    # compare the current student's file with the other student's file
                    diff Submissions/$file_1/$file_1.sh Submissions/$file_2/$file_2.sh --ignore-all-space > Submissions/$file_1-$file_2-diff.txt;
                    
                    # count the number of lines in the diff file(this is the number of differences)
                    val=$(grep -c "^[<>]" Submissions/$file_1-$file_2-diff.txt);
                    if [ $val -eq 0 ]; then             # if the number of differences is 0
                        score=$((-1 * $score));         # score is set to -score
                    fi
                    rm Submissions/$file_1-$file_2-diff.txt;            # remove the diff file
                fi
            fi
        done
    else
        echo "No file $file_1.sh in $file_1 directory Student $file_1 is not graded"
        score=0;            # if the file is not a directory or there is no sh file in the directory, set the score to 0
    fi

    echo "$file_1, $score" >> output.csv;            # echo students_id, score in a file

done
echo "Success!!!!";