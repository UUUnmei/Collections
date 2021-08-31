-- 16位二选一
-- vwf4
library ieee;
use ieee.std_logic_1164.all;

entity c21_16 is
port(en: in std_logic;  --en地址线 为0选d0 为1选d1
		d0, d1: in std_logic_vector (15 downto 0);
		q: out std_logic_vector (15 downto 0));
end c21_16;

architecture work of c21_16 is

component c21 is
port(en: in std_logic;  --en地址线
		d1, d0: in std_logic;
		q: out std_logic);
end component;

begin 
	
	u1  : c21 port map(en, d1(0)  , d0(0),  q(0));
	u2  : c21 port map(en, d1(1)  , d0(1),  q(1));
	u3  : c21 port map(en, d1(2)  , d0(2),  q(2));
	u4  : c21 port map(en, d1(3)  , d0(3),  q(3));
	u5  : c21 port map(en, d1(4)  , d0(4),  q(4));
	u6  : c21 port map(en, d1(5)  , d0(5),  q(5));
	u7  : c21 port map(en, d1(6)  , d0(6),  q(6));
	u8  : c21 port map(en, d1(7)  , d0(7),  q(7));
	u9  : c21 port map(en, d1(8)  , d0(8),  q(8));
	u10 : c21 port map(en, d1(9)  , d0(9),  q(9));
	u11 : c21 port map(en, d1(10) , d0(10), q(10));
	u12 : c21 port map(en, d1(11) , d0(11), q(11));
	u13 : c21 port map(en, d1(12) , d0(12), q(12));
	u14 : c21 port map(en, d1(13) , d0(13), q(13));
	u15 : c21 port map(en, d1(14) , d0(14), q(14));
	u16 : c21 port map(en, d1(15) , d0(15), q(15));
	
end work;