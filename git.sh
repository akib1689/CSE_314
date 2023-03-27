#check the number of arguments as message
if [ $# -eq 1]
then 
    echo "Please input commit message....."
else
    git add . 
    git commit -m "$1"
    git push origin master
fi
