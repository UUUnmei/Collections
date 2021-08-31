library ieee;
use ieee.std_logic_1164.all;

entity dr is
port(edr, idr, clk : in std_logic;
	  dr_in : in std_logic_vector(15 downto 0);
	  dr_out : out std_logic_vector(15 downto 0));
end dr;

architecture work of dr is

component LS373 is
port(oc, en: in std_logic;  -- oc 输出控制低有效，=1时输出高阻态；en使能，=1可写入
		d: in std_logic_vector (7 downto 0);
		q: out std_logic_vector (7 downto 0));
end component;

signal E : std_logic := '0';

begin
	
	E <= idr and clk;
	u1: LS373 port map(edr, E, dr_in(15 downto 8), dr_out(15 downto 8));
	u2: LS373 port map(edr, E, dr_in(7 downto 0), dr_out(7 downto 0));

end architecture work;