![](https://cdn.jsdelivr.net/gh/UUUnmei/Figurebed/img/20201226115606.png)  
基于C# + WPF  ~~究极现学现卖~~  
目前（2020.12.26）支持四种识别模式  
建议从 bin/Release 中的.exe打开  
点击 载入已有图灵机 从target文件夹中选择  
0n1n.txt --- n个0 n个1 n>=1  
wcw.txt  --- 左右相同的a、b串中间一个c   
m-n.txt  --- 计算m-n，都是正整数，m<=n时为0，输入000100表示3-2
log2n.txt -- 计算以2为底的对数，向下取整，输入aaa 表示![](https://latex.codecogs.com/gif.latex?log_2(3))  输出 一个b 表示1  

每个txt中
第一行K  
第二行![](https://latex.codecogs.com/gif.latex?\\sum)  
第三行![](https://latex.codecogs.com/gif.latex?\\Gamma)  
第四行开始状态 q0  
第五行中止状态 qf  
接下来是转移函数
形如 ![](https://latex.codecogs.com/svg.latex?\\delta(q, a)=(p,b,Z))
其中的五个字母按顺序给出，Z只能为L,R,S表示读头的移动  
以上一行若有多个都需要空格分开
