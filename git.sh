#check the number of arguments as message
if [ $# -eq 0]
then 
    echo "Please input commit message....."
else
    git add . 
    git commit -m "$1"
    git push origin master
fi
