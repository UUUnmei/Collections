-- 16位四选一
-- vwf5

library ieee;
use ieee.std_logic_1164.all;

entity c41_16 is
port(en: in std_logic_vector(1 downto 0);  --en地址线
		d0, d1, d2, d3: in std_logic_vector (15 downto 0);
		q: out std_logic_vector (15 downto 0));
end c41_16;

architecture work of c41_16 is

component c21_16 is
port(en: in std_logic;  --en地址线 为0选d0 为1选d1
		d0, d1: in std_logic_vector (15 downto 0);
		q: out std_logic_vector (15 downto 0));
end component;

signal r1, r2 : std_logic_vector(15 downto 0);

begin 
	
	u1 : c21_16 port map(en(0), d0, d1, r1);
	u2 : c21_16 port map(en(0), d2, d3, r2);
	u3 : c21_16 port map(en(1), r1, r2, q);
	
end work;