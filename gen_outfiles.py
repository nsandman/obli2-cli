import os

# what file extensions are code, and what to append onto the target
# name so make will know what compiler to use
code_files = {
    "c": ""
}

# generate two Makefile variables: targets (all the files to build)
# and VPATH, the directories make will search
# start must be first so it's at the beginning of the binary
targets = "TARGETS ="
vpath   = "VPATH = "
incpath = "INCFLAGS ="
ignpath = "IGNFILES ="

# source directories relative and absolute
src_rel = "src"
sources = os.path.join(os.getcwd(), src_rel)

# generate target name based on file name
def gen_target(file_name, file_ext):
    global ignpath
    try:
        return " " + file_name + code_files[file_ext] + "." + file_ext
        #return " out/" + file_name + code_files[file_ext] + ".o"
    except KeyError:
        if (file_ext != "DS_Store"):
            ignpath += " " + file_name + "." + file_ext
        return None

if __name__ == "__main__":
    for folder, _, files in os.walk(sources):
        relative_folder = os.path.relpath(folder, os.getcwd())
        if relative_folder != src_rel:    # don't prepend : if first element
            vpath += ":"
        vpath += relative_folder
        incpath += " -I" + relative_folder

        for file in files:
            file_parts = file.split(".")

            file_name  = "".join(file_parts[:-1])
            file_ext   = file_parts[-1]

            # we already manually added start
            if file_name != "start":
                new_target = gen_target(file_name, file_ext)
                if new_target != None:
                    targets += new_target

    # write our variables to our output files
    f = open("outfiles.local", "w")
    f.write(targets + "\n")
    f.write(vpath + "\n")
    f.write(incpath + "\n")
    f.write(ignpath)
    f.close()

