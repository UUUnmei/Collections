-- 16位减法器
-- 补码
-- 原理可参考第四章ppt P168
-- vwf14



library ieee;
use ieee.std_logic_1164.all;


entity LAsuber16 is
port(A, B: in std_logic_vector(15 downto 0);		-- A - B
	  S : out std_logic_vector(15	 downto 0);
	  Cout: out std_logic);							-- Cout 为1表示有借位
end LAsuber16;

architecture work of LAsuber16 is

component LAadder16 is 
port(A, B: in std_logic_vector(15 downto 0);
	  Cin: in std_logic;	
	  S : out std_logic_vector(15	 downto 0);
	  Cout: out std_logic);
end component;

-- ！这里由于是加法器改的减法器，进位位方面不太能直接用，这里不想细究了直接上比较器
-- ! 在比较的时候认为是无符号数
component cmp is
port(A, B: in std_logic_vector(15 downto 0);		
	  ALBout, AEBout, AGBout : out std_logic);			
end component;

signal LT, EQ, GT : std_logic := '0';

signal Bn: std_logic_vector(15 downto 0);
signal cc : std_logic;

begin
	c : cmp port map(A, B, LT, EQ, GT);
	Bn <= not B;
	Cout <= LT;
	u1 : LAadder16 port map(A, Bn, '1', S, cc);
	
end architecture work;

