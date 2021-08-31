-- 左移运算
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity cal_shl is
port(A, B: in std_logic_vector(15 downto 0);		-- A shl B
	  S : out std_logic_vector(15	 downto 0));			
end cal_shl;

architecture work of cal_shl is


begin
	

	S <= A when B = x"0000" else
		  A(14 downto 0) & '0' when B = x"0001" else
		  A(13 downto 0) & (1 downto 0 => '0') when B = x"0002" else
		  A(12 downto 0) & (2 downto 0 => '0') when B = x"0003" else
		  A(11 downto 0) & (3 downto 0 => '0') when B = x"0004" else
		  A(10 downto 0) & (4 downto 0 => '0') when B = x"0005" else
		  A(9 downto 0) & (5 downto 0 => '0')when B = x"0006" else
		  A(8 downto 0) & (6 downto 0 => '0') when B = x"0007" else
		  A(7 downto 0) & (7 downto 0 => '0') when B = x"0008" else
		  A(6 downto 0) & (8 downto 0 => '0') when B = x"0009" else
		  A(5 downto 0) & (9 downto 0 => '0') when B = x"000a" else
		  A(4 downto 0) & (10 downto 0 => '0') when B = x"000b" else
		  A(3 downto 0) & (11 downto 0 => '0') when B = x"000c" else
		  A(2 downto 0) & (12 downto 0 => '0') when B = x"000d" else
		  A(1 downto 0) & (13 downto 0 => '0') when B = x"000e" else
		  A(0) & (14 downto 0 => '0') when B = x"000f" else
		  x"0000";
		  
	
end architecture work;

