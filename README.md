# StudentManage

## Build
To use `make` in Windows, I recommend [Chocolatey](https://chocolatey.org/install).  
After installing Chocolatey :
```bash
choco install make
```
then you can build the project :
```bash
make
make reload
```
to debug, use `make debug` instead of `make`

## Usage
Before first run of the server, you should run `reload.exe` first to create a binary data library from `TestData`. After reloading binary data, the server is ready to run.
```bash
reload.exe
StudentManage.exe
```
Make sure to recompile and run `reload.exe` after any modification to `TestData.json`  