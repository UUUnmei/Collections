-- vwf22

library ieee;
use ieee.std_logic_1164.all;

entity LS377 is 
port(en, clk : in std_logic;		-- en 使能低有效
		d : in std_logic_vector(15 downto 0);
		q : out std_logic_vector(15 downto 0));
end LS377;

architecture work of LS377 is

signal enn : std_logic := '0';
signal r_q : std_logic_vector(15 downto 0);
begin


	enn <= not en;
	q <= r_q;
	process(clk)
	begin
	if(rising_edge(clk) and enn = '1') then
		r_q <= d;
	end if;
	end process;

end architecture work;