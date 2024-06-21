# Werewolf
Welcome to our Werewolf project! We're glad to see you here.
Please, read the ```Quick Start``` section to be able to contribute to this project (setup configuration mostly).

## Documentation
To review the complete documentation go to the following link
https://drive.google.com/drive/folders/1U9WqZ5GmvYSgROKsc-7zoD3VeNtPHer2?usp=sharing

## References
###WebSockets
https://beej.us/guide/bgnet/pdf/bgnet_usl_c_1.pdf

## Quick Start
### 1. Clone this repository on your computer. 

To do so, just click in the code button at the right-top corner of the repo, just below the watch counter. Copy to your clipboard the HTTPS url using the icon. 

![Clone Button](img/qs_01.png)

On your VSCode with a new empty folder open, select the option of Source Control (it looks like branches on the left) and hit the option ```Initialize Repository```. If git is well configured on your machine, the icon should disappear.

![Init Repository](img/qs_02.png)

Then, click the three dots in the corner and hit the option ```Remote -> Add Remote```. It will ask you for the url, here's where you paste the one you copied to your clipboard before. Then will ask you for a name. It doesn't really matter, you can just type something standard as ```origin``` and it would do the job. 

![Add remote](img/qs_03.png)

In the bottom left corner of your vscode select the branch ```main```. Click on it and select in the box ```<remote>/main``` (in this case, ```origin/main```) and it should automatically start with the pulling of files from GitHub.

![Pull files](img/qs_04.png)

After that, you should have the latest version of that branch on your computer. (Check this in the files for that folder).

![Pull files](img/qs_05.png)

### 2. Run the build_run.ps1 script

To ensure you're able to run the Net Blocks provided in the net folders, you might check the execution of ```build_run.ps1``` script. To run it, just open a Powershell instance on the project folder (could be a terminal on the vscode instance of the previous step) and type:
```ps1
.\scripts\build_run.ps1
```

If it hit some error it could be related with the previligies of PS to ecxecute PS1 files manually. In that case, please read the output error and try to fix it and try again (you can contact me @buronsuave if you have problems on this one). 

If the script goes well, it should compile both client and server and put the exc files in a new folder call ```.build``` in the same project directory. Then, it will attempt to execute them creating an instance of the server and three client instances. The first client instance will be set as the **main host**, and it has the ability to change the stage of the game to an init status. You should see a similar output that the image once you hit the enter key in the main host terminal.

![Running test script](img/qs_06.jpeg)

If this is the behavior you're seeing, you're done and ready to start contribuiting to this precious code. 

Happy hacking!

## Net Blocks

```TODO``` : Here's where I'm going to upload basic documentation of usage of the developed network blocks for client/server socket implementations.
