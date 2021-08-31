-- 16位比较器
-- vwf23
library ieee;
use ieee.std_logic_1164.all;

entity cmp is
port(A, B: in std_logic_vector(15 downto 0);		
	  ALBout, AEBout, AGBout : out std_logic);			
end cmp;

architecture work of cmp is

component LS85 is
port(A, B: in std_logic_vector(3 downto 0);		
	  ALBin, AEBin, AGBin : in std_logic;
	  ALBout, AEBout, AGBout : out std_logic);			
end component;

signal r1, r2,r3,r4,r5,r6,r7,r8,r9 : std_logic;

begin

	u1 : LS85 port map(A(3 downto 0), B(3 downto 0), '0', '1', '0', r1, r2, r3);
	u2 : LS85 port map(A(7 downto 4), B(7 downto 4), r1, r2, r3, r4, r5, r6);
	u3 : LS85 port map(A(11 downto 8), B(11 downto 8), r4, r5, r6, r7, r8, r9);
	u4 : LS85 port map(A(15 downto 12), B(15 downto 12), r7, r8, r9, ALBout, AEBout, AGBout);
	
end architecture work;

