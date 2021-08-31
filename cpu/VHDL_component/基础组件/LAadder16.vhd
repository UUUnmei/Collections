-- 16位超前进位加法器
library ieee;
use ieee.std_logic_1164.all;

entity LAadder16 is 
port(A, B: in std_logic_vector(15 downto 0);
	  Cin: in std_logic;	
	  S : out std_logic_vector(15	 downto 0);
	  Cout: out std_logic);
end LAadder16;

architecture work of LAadder16 is

component LAadder4 is
port(A, B: in std_logic_vector(3 downto 0);
	  Cin, En: in std_logic;	-- En 低有效控制端 为0有效 为1则输出全1
	  S : out std_logic_vector(3	 downto 0);
	  Cout: out std_logic);
end component;


signal ss : std_logic_vector(15 downto 0) := (others => '0');
signal cc : std_logic_vector(3 downto 0) := (others => '0');
signal En : std_logic := '0';

begin 

	u1 : LAadder4 port map(A(3 downto 0), B(3 downto 0),   Cin, En, ss(3 downto 0), cc(0));
	u2 : LAadder4 port map(A(7 downto 4), B(7 downto 4), cc(0), En, ss(7 downto 4), cc(1));
	u3 : LAadder4 port map(A(11 downto 8), B(11 downto 8), cc(1), En, ss(11 downto 8), cc(2));
	u4 : LAadder4 port map(A(15 downto 12), B(15 downto 12), cc(2), En, ss(15 downto 12), cc(3));

	S <= ss;
	Cout <= cc(3);

end architecture work;