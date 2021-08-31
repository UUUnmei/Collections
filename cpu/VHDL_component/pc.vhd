-- vwf19

library ieee;
use ieee.std_logic_1164.all;

entity pc is
port(
	clr, clk, ipc: in std_logic;		--clr低有效， ipc使能高有效
	ijmp : in std_logic;
	target : in std_logic_vector(7 downto 0); -- 跳转指令的地址输入
	pc_out : out std_logic_vector(7 downto 0));
end pc;

architecture work of pc is

component LS161 is
port(clr, clk, enp, ent, ld: in std_logic;
		d : in std_logic_vector (3 downto 0);
		q : out std_logic_vector (3 downto 0);
		rco : out std_logic);
end component;

signal buf : std_logic_vector(7 downto 0) := (others => '0');
signal rco1, rco2, en0, en1 : std_logic := '0';

-- , rco3, rco4
begin
	
	en0 <= ipc or not ijmp;
	en1 <= (rco1 and ipc) or not ijmp;
	
	
	u1: LS161 port map(clr, clk, en0, '1', ijmp, target(3 downto 0), buf(3 downto 0), rco1);
	u2: LS161 port map(clr, clk, en1, '1', ijmp, target(7 downto 4), buf(7 downto 4), rco2);
--	u3: LS161 port map(clr, clk, rco2, '1', '1', "0000", buf(11 downto 8), rco3);
--	u4: LS161 port map(clr, clk, rco3, '1', '1', "0000", buf(15 downto 12), rco4);
	
	pc_out <= buf;

end architecture work;