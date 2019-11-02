****Operation-System****

**signals Ex:**

write a program that runs a 5 processes which send to each other
signals using the kill command. Each process prints a msg with his PID
when he is ready, The signals activate the procedure
sigCathcher which sends a signal to the next process (his sibling). Each
child process exits after receiving the signal. The parent collects all the
zombies and then exits

_Output example:_

![](https://imagizer.imageshack.com/img922/9158/ddowK0.jpg)

**Pipes Ex:**

The parent receives a string from the user at a maximum length of
20 characters, and sends it to the child process. When the son process gets the string he encrypt to MD5 (hashing)
and returns the encrypted string to the parent process and sends a signal to "checkMD5" function that checks whether
the encryption succeeded (the encrypted string length should be 32 characters) If so, the parent process prints the string
And kill the son process.


<img src="https://imagizer.imageshack.com/img924/4838/ZOuPx3.jpg" width="500" height="70" >
