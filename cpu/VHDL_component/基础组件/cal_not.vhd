-- 取反运算
library ieee;
use ieee.std_logic_1164.all;


entity cal_not is
port(A: in std_logic_vector(15 downto 0);		-- A not B
	  S : out std_logic_vector(15	 downto 0));			
end cal_not;

architecture work of cal_not is

begin
	
	S <= not A;
	
end architecture work;

