library ieee;
use ieee.std_logic_1164.all;


--in_from_dbus  表示是否接受总线输入  低有效！ 
--out_to_dbus	  表示是否向总线输出   高有效！
--out_to_alu	  表示是否向alu输出（不过其实还隔了一个选择器） 好像不太需要

entity Reg is 
port(clk, in_from_dbus, out_to_dbus: in std_logic;
			-- out_to_alu
		d : in std_logic_vector(15 downto 0);
		qalu, qbus : out std_logic_vector(15 downto 0));
end Reg;

architecture work of Reg is

component LS377 is 
port(en, clk : in std_logic;		-- en 使能低有效
		d : in std_logic_vector(15 downto 0);
		q : out std_logic_vector(15 downto 0));
end component;

component LS244 is 
port(gn1, gn2 : in std_logic;  -- gn1 gn2均为0时有效
		d : in std_logic_vector(15 downto 0);
		q : out std_logic_vector(15 downto 0));
end component;

signal buf : std_logic_vector(15 downto 0) := (others => '0');
signal en : std_logic := '1';

begin
	
	u1 : LS377 port map(in_from_dbus, clk, d, buf);
	qalu <= buf;
	
	en <= not out_to_dbus;
	u2 : LS244 port map(en, en, buf, qbus);
--	qbus <= buf when out_to_dbus = '1' else (others => 'Z');

	

end architecture work;