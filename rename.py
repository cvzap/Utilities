# Renames files in a directory if using a windows system change the slashes
# Useful if files are named similar to get a uniformed naming convention


import os

# provides the path to the folder
src = "[PATH TO FOLDER]"

# grabs an enumerated array of file names 
for d, file in enumerate(os.listdir(src)):
    
    # change these values to match what you are working with
    org = src + "\\" + file

    # changes name to file number and adds an a letter ie a1.png
    # change this to match the file naming convention you choose
    dest = src + "\\a" + str(d) + ".png"

    # does the renaming 
    os.rename(org, dest)
