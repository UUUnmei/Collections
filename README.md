# Collections
some programs

## Turing Machine1.0  
来自计算理论  
一个简易的不怎么通用的通用图灵机  
详见其中的readme

## compiler
来自编译原理课设  
支持基本的类c语法，变量定义、赋值、表达式、条件、循环，无函数调用  
仅支持整数类型，最终生成8086平台的汇编代码，可在dosbox和masm环境下运行

## re_scanner
一个基于正则表达式的词法分析器  
正则表达式实现的很粗糙，写的也有些痛苦，  
就是从RE类定义规则->带空边的nfa->nfa->dfa，还没有dfa最小化等优化。。  
仅仅作为自动机理论的练习吧。。。难以回首

## c_project
c课设  
会员卡管理系统，  
（界面是键盘选择、切换的效果，算是一点特殊之处吧。。）

## cpp_project
c++课设，QT  
简易飞机大战，（难度设置的不太好。。。

## DS_MST_by_WDS
数据结构课设、QT、图形界面  
主要为了展示最小生成树的两种算法：Prim和Kruskal  
感谢wxf、dyf两位大佬队友

## cpu
数电课设，16位模型机，可支持顺序、条件跳转只写了一种不过也能支持分支、循环了  
存储器是只读的、比较可惜

## fs
操作系统课设，类unix文件系统
空闲块管理基于bitmap
混合索引，多级目录，对用户权限有一点支持，默认管理员账号密码root
“硬盘”和内存间有一层LRU缓存
用户权限等文件“元数据”位于文件数据块前100个字节的FileHeader结构
支持基本文件、目录操作，cd,ls,touch,mkdir,open,close,cat,写文件(<,<<),stat,cp,mv,rm[-r],ln[-s],chmod,format
