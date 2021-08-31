library ieee;
use ieee.std_logic_1164.all;

entity LS244 is 
port(gn1, gn2 : in std_logic;  -- gn1 gn2均为0时有效
		d : in std_logic_vector(15 downto 0);
		q : out std_logic_vector(15 downto 0));
end LS244;

architecture work of LS244 is

signal gn : std_logic := '0';

begin


	gn <= not gn1 and not gn2;

	process(d, gn)
	begin
	if(gn = '1') then
		q <= d;
	else
		q <= (others => 'Z');
	end if;
	end process;

end architecture work;