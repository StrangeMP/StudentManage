import os
def NewFile(filepath: str):
    base_file_name, extension = os.path.splitext(filepath)

    # Initialize index
    index = 0

    # Check if the file exists
    while os.path.isfile(filepath):
        # If it exists, increment the index and update the file path
        index += 1
        filepath = os.path.join(os.getcwd(),
                                f"{base_file_name}_{index}{extension}")

    return filepath
