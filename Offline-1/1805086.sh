# At first list the files in the directory submissions
# Then, for each file in the directory, give the permission to execute
# Then, for each file in the directory, execute the file and save the output in a file
# Then, for each file in the directory, remove the file
# Then, compare the output with other files in the directory
# Then, for each file in the directory, remove the file


#echo students_id, score in a file
echo "students_id, score" > output.csv

files=$(ls Submissions)         # list the files in the directory submissions


for file_1 in $files; do        # for each file in the directory
    if [ -d Submissions/$file_1 ]; then         # if the file is a directory proceed
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
        if [ $# -eq 1 ]; then                # if the number of arguments is 1
            score=$(($1 - $val));            # subtract the number of differences from given score
        else
            score=$((100 - $val));           # subtract the number of differences from 100
        fi
        if [ $score -lt 0 ]; then           # if the score is less than 0
            score=0;                        # set the score to 0
        fi
    fi

    #now copy checker inmlementation
    #compare the current student's file with all the other students' files
    #if there is a match, then the student gets -score
    for file_2 in $files; do
        if [ -d Submissions/$file_2 ]; then             # if the file is a directory
            if [ "$file_1" != "$file_2" ]; then         # if the file is not the current student's file

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
    echo "$file_1, $score" >> output.csv;            # echo students_id, score in a file

done
echo "Success!!!!";