
import yaml
import os
import shutil
import warnings

#  This script is made because Arduino for whatever reason **CANNOT** build from files in different
#  directories. This is a really dumb limitation, but it's a better idea to remain organized in folders
#  and just have a helper script pull, refactor, and dump all of the necessary project files into a
#  single folder for easy use.

def copy_directory(directory: str, destination_folder: str = "./build") -> list[str]:
    """
    :param directory: the path to the top-level directory to copy from.
    :param destination_folder: the path in which all the files are copied to.
    :return: A list of the paths to the new files.
    """
    out_files = []
    for folder, _, files in os.walk(directory):
        for file in files:
            file = str(os.path.join(folder, file))
            if os.path.exists(file):
                warnings.warn(f"A file already exists at {file}; overwriting it. \n (However, this is intentional if making test-suites!)")
                os.remove(os.path.join(destination_folder, os.path.split(file)[1]))
            shutil.copy(file, destination_folder)
            out_files.append(str(os.path.join(destination_folder, os.path.split(file)[-1])))
    return out_files

def update_includes(file: str) -> None:
    """
    :param file: A file to update the `#include`'s.
    :return: Nothing; although if the file is a valid C++ file, the `#include`'s have been updated.
    """
    if os.path.splitext(file)[1] not in (".hpp", ".h", ".hh", ".cpp", ".c"):  # check if file is a C++ or C file
        return

    file_contents = open(file, 'r').read().split('\n')
    updated_lines = []
    for line in file_contents:
        if "#include" not in line:  # ensure that the line refers to an "#include" preprocessor directive
            updated_lines.append(line)
            continue
        if "<" in line:  # ensure that the "#include" isn't of a standard library
            updated_lines.append(line)
            continue
        library = line[line.find('"')+1: line.rfind('"')]
        library = os.path.split(library)[-1]  # remove the path to the file
        updated_lines.append(line[:line.find('"')+1:] + library + line[line.rfind('"'):])

    with open(file, 'w+') as f:
        f.write('\n'.join(updated_lines))

def make_build(yaml_path: str = "build_directories.yaml", build_directory: str = "./build"):
    """
    :param yaml_path: path to the yaml with the build directory information.
    :param build_directory: path where the build directories will be made.
    :return: Nothing.
    """
    with open(yaml_path) as yaml_file:
        build_information = yaml.safe_load(yaml_file)
    for build_folder, copy_folders in build_information.items():
        target_folder = os.path.join(build_directory, build_folder)
        os.mkdir(target_folder)
        for copy_folder in copy_folders:
            copied_files = copy_directory(copy_folder, target_folder)
            for file in copied_files:
                update_includes(file)

if __name__ == "__main__":
    if os.path.exists("build"):
        shutil.rmtree("build")
    if not os.path.exists("build"):  # This isn't useless; if "build" is empty, the first "if" won't run.
        os.mkdir("build")
    make_build()

