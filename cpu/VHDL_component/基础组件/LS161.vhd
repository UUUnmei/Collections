-- 同步4位二进制计数器 （清零异步，预置数同步）
 
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity LS161 is
port(clr, clk, enp, ent, ld: in std_logic;
		d : in std_logic_vector (3 downto 0);
		q : out std_logic_vector (3 downto 0);
		rco : out std_logic);
end LS161;

architecture work of LS161 is
signal cnt : std_logic_vector (3 downto 0);

begin
	process(clk, clr)
	begin
	if(clr = '0') then cnt <= "0000";
	elsif (rising_edge(clk)) then 
		if(ld = '0') then 
			cnt <= d;
		elsif (enp = '1' and ent = '1') then
			cnt <= cnt + '1';  -- ！！！需要 unsigned 库
		end if;
	end if;
	end process;
	rco <= cnt(0) and cnt(1) and cnt(2) and cnt(3) and ent;
	q <= cnt;
end work;