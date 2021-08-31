library ieee;
use ieee.std_logic_1164.all;

entity DFF74 is
port(D, prn, clrn, clk:in std_logic;
		Q, Qn: out std_logic);
end DFF74;

architecture work of DFF74 is
signal qq : std_logic;

begin 

	process(clrn, clk, prn)
	begin
	if clrn = '0' then qq <= '0';
	elsif prn = '0' then qq <= '1';
	elsif rising_edge(clk) then qq <= D;
	end if;
	end process;
	
	Q <= qq;
	Qn <= not qq;

end work;