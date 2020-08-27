import os

# what file extensions are code, and what to append onto the target
# name so make will know what compiler to use
code_files = ["c", "go"]
listen_files = ["h"]
no_process = ["main.c", "test.c"]

# source directories relative and absolute
srces_mod = {
    "client": False,
    "daemon": False,
    "lib": True
}
srces_rel = list(srces_mod.keys())
mod_extensions = {
    "c": ""
}
out_path = "outfiles.local"

# generate target name based on file name
def gen_target(file_name, file_ext, modify=False):
    if "{}.{}".format(file_name, file_ext) in no_process:
        return None

    if modify:
        try:
            return " out/" + file_name + mod_extensions[file_ext] + ".o"
        except KeyError:
            return None
    else:
        return " " + file_name + "." + file_ext

vpath = "VPATH = "
vpath_modified = False

if __name__ == "__main__":
    out_file = open(out_path, "w")

    for src_rel in srces_rel:
        sources = os.path.join(os.getcwd(), src_rel)

        # generate two Makefile variables: targets (all the files to build)
        # and VPATH, the directories make will search
        # start must be first so it's at the beginning of the binary
        src_name_makefile = src_rel.upper() + "_"

        targets = src_name_makefile + "TARGETS ="
        incpath = src_name_makefile + "INCFLAGS ="
        ignpath = src_name_makefile + "IGNFILES ="

        for folder, _, files in os.walk(sources):
            relative_folder = os.path.relpath(folder, os.getcwd())
            if vpath_modified:    # don't prepend : if first element
                vpath += ":"
            vpath += relative_folder
            incpath += " -I" + relative_folder
            vpath_modified = True

            for file in files:
                file_parts = file.split(".")

                file_name  = "".join(file_parts[:-1])
                file_ext   = file_parts[-1]

                # we already manually added start
                if file_name != "start":
                    new_target = gen_target(file_name, file_ext, srces_mod[src_rel])

                    if new_target:
                        if (file_ext in code_files):
                            targets += new_target
                        elif file_ext in listen_files:
                            ignpath += new_target

        # write our variables to our output files
        out_file.write(targets + "\n")
        out_file.write(incpath + "\n")
        out_file.write(ignpath + "\n")

    out_file.write(vpath + "\n")     
    out_file.close()

