Bouyomi-chan only works on Windows if you set your Operating System locale to Japanese. 
This means that you not only have to use the Operating System Language but also their Codepage 932 Shift JIS.

The reason for this is that the Synthesizer Library Bouyomi-chan uses, named AquesTalk, didn't support Unicode Input. 
In Bouyomi-chan, the Library Signature is labeled with a native C# String, which means it's using the default string encoding.

If you are running your Operating System in Japanese, it works just fine. 
However, if your Operating System is using a different Codepage, Bouyomi-chan sends the string in your codepage to the library, which doesn't work.

What we can still do is to extend Bouyomi-chan with an encoding feature. 
The question is, how can we do that without breaking any copyright laws? 

The solution is to not touch their program at all and build our own library that provides the same function names as AquesTalk. 
Then, we can rename AquesTalk.dll to _AquesTalk.dll and copy our dll as AquesTalk.dll into each voice directory.

If Bouyomi-chan starts, it will load our library instead, which loads the original Library and 
proxies the calls while it encodes the string to your operating system string encoding.

For those who think it's too much work to rename eight different files and copy the same file eight times, 
I have also deployed a PowerShell script for you.

With this you can use Bouyomi-chan, while your Operating System stays in your native Language

Just download Boyoumi, extract it somewhere download the latest x86 build and double click any "patch" file which works for you
choose the Bouyomi Directory, apply and it should be patched then.

If you can't run any you have to execute the js file via CMD: `wscript.exe /E:jscript patch.js`