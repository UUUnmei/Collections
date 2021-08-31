-- 算术右移运算
library ieee;
use ieee.std_logic_1164.all;

entity cal_sar is
port(A, B: in std_logic_vector(15 downto 0);		-- A sar B
	  S : out std_logic_vector(15	 downto 0));			
end cal_sar;

architecture work of cal_sar is


begin
	
	S <= A when B = x"0000" else
		  A(15) & A(15 downto 1) when B = x"0001" else
		  (1 downto 0 => A(15)) & A(15 downto 2) when B = x"0002" else
		  (2 downto 0 => A(15)) & A(15 downto 3) when B = x"0003" else
		  (3 downto 0 => A(15)) & A(15 downto 4) when B = x"0004" else
		  (4 downto 0 => A(15)) & A(15 downto 5) when B = x"0005" else
		  (5 downto 0 => A(15)) & A(15 downto 6) when B = x"0006" else
		  (6 downto 0 => A(15)) & A(15 downto 7) when B = x"0007" else
		  (7 downto 0 => A(15)) & A(15 downto 8) when B = x"0008" else
		  (8 downto 0 => A(15)) & A(15 downto 9) when B = x"0009" else
		  (9 downto 0 => A(15)) & A(15 downto 10) when B = x"000a" else
		  (10 downto 0 => A(15)) & A(15 downto 11) when B = x"000b" else
		  (11 downto 0 => A(15)) & A(15 downto 12) when B = x"000c" else
		  (12 downto 0 => A(15)) & A(15 downto 13) when B = x"000d" else
		  (13 downto 0 => A(15)) & A(15 downto 14) when B = x"000e" else
		  (14 downto 0 => A(15)) & A(15) when B = x"000f" else
		  x"0000";
		  
	
end architecture work;

