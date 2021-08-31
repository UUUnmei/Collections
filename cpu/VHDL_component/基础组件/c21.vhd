-- 二选一
-- vwf2
library ieee;
use ieee.std_logic_1164.all;

entity c21 is
port(en: in std_logic;  --en地址线 0选d(0) 1选d(1)
		d1, d0: in std_logic;
		q: out std_logic);
end c21;

architecture work of c21 is

begin 

		q <= (not en and d0) or (en and d1);
	
end work;