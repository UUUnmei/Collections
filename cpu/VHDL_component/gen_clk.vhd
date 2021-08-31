library ieee;
use ieee.std_logic_1164.all;

entity gen_clk is
port(clk_in, en : in std_logic;			-- en低有效
		clk_out : out std_logic);
end gen_clk;


architecture work of gen_clk is

signal tmp : std_logic := '0';

begin
	
	process(clk_in)
	variable cnt : integer range 0 to 12500000 := 0;
	begin
	if(en = '1') then
		tmp <= '0';
	elsif(rising_edge(clk_in)) then
		if(cnt = 12500000) then					-- 分频系数 0.5s 12500000 0.25s 6250000
			cnt := 0;
			tmp <= not tmp;
		else
			cnt := cnt + 1;
		end if;
	end if;
	end process;
	
	clk_out <= tmp;

--	clk_out <= clk_in;
	


end architecture work;