# Prerequisites:
# Gemini API
# pip install -U google-generativeai

import json
import os
import google.generativeai as genai


class model:

    def get_api_key(s):
        userprofile = os.getenv("USERPROFILE")
        file_path = os.path.join(userprofile, ".API_KEY.json")
        # Check if the file exists
        if not os.path.isfile(file_path):
            # If not, create the file with the specified format
            with open(file_path, "w") as file:
                json.dump({"api_key": "replace with Your-API-KEY"},
                          file,
                          indent=4)
            print(
                f"File .API_KEY.json has been created at {file_path}. Please edit this file with your API key and try again."
            )
            exit()
        with open(file_path) as f:
            data = json.load(f)
        return data["api_key"]

    def __init__(self) -> None:
        # Initializing the API
        genai.configure(api_key=self.get_api_key(), transport="rest")
        self.model = genai.GenerativeModel("gemini-pro")
        self.chat = self.model.start_chat(history=[])

    def beginChat(self, prompt: str, logFilePath: str = None):
        if logFilePath:
            try:
                file = open(logFilePath, "w")  # Attempt to open the file
            except FileNotFoundError:
                print(
                    f"Error: File '{logFilePath}' not found. Printing to stdout only."
                )
            except OSError as err:
                print(f"Error opening file: {err}. Printing to stdout only.")
        else:
            file = None
        response = self.chat.send_message(prompt)
        if file:
            file.write(response.text)
        print(response.text)
        print("_" * 80)
        while True:
            reply = input(
                'Enter your response, enter "exit" to exit, simply press Enter to continue: '
            )
            if reply == "":
                reply = "continue"
            elif reply == "exit":
                break
            print("_" * 80)
            response = self.chat.send_message(reply)
            print(response.text)
            print("_" * 80)
            if file:
                file.write(response.text)
        if file:
            file.close()


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
