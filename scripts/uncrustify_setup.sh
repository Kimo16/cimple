#!/bin/sh

PRE_COMMIT_PATH="./../.git/hooks/pre-commit"

#Checking for an existing pre-config file
if [ ! -f ${PRE_COMMIT_PATH} ]; then

    echo "${PRE_COMMIT_PATH} not found!"
    echo "Creating..."
    touch ${PRE_COMMIT_PATH}

    # Creating the pre config script
    echo "#!/bin/sh" > ${PRE_COMMIT_PATH}
fi

#Adding an uncrustify execute line
echo "sh ./scripts/uncrustify.sh" >> ${PRE_COMMIT_PATH}

#Adding permissions
sudo chmod 744 ${PRE_COMMIT_PATH}

echo "Completed with success"

exit 0
