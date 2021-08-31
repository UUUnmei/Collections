-- vwf20

library ieee;
use ieee.std_logic_1164.all;

entity mar is
port(
	imar, clk : in std_logic ; -- imar使能低有效
	mar_in : in std_logic_vector(7 downto 0);
	mar_out : out std_logic_vector(7 downto 0));
end mar;

architecture work of mar is

signal buf : std_logic_vector(7 downto 0) := (others => '0');

begin
	
	mar_out <= buf;
	
	process(clk)
	begin
	if(rising_edge(clk) and imar = '0')  then
	-- 在目前的设计中，
	-- 一次读写是2字节，故每次起始地址应该都是偶地址
	-- 比如pc=0，对应0和1两个字节；pc=3对应6和7两个字节
	-- 这里mar给出偶地址，其后的奇地址在存储器部分
		buf <= mar_in(6 downto 0) & '0';		
	end if;
	end process;
end architecture work;