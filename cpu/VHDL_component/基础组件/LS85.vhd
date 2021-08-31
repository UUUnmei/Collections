-- 74LS85 比较器

library ieee;
use ieee.std_logic_1164.all;


entity LS85 is
port(A, B: in std_logic_vector(3 downto 0);		
	  ALBin, AEBin, AGBin : in std_logic;
	  ALBout, AEBout, AGBout : out std_logic);			
end LS85;

architecture work of LS85 is


signal gate1, gate2,gate3,gate4,gate5 : std_logic;
signal gate6, gate7, gate8, gate9, gate10 : std_logic;
signal gate11, gate12,gate13, gate14,gate15 : std_logic;
signal gate16, gate17,gate18, gate19,gate20 : std_logic;
signal gate21,gate22,gate23, gate24,gate25 : std_logic;
signal gate26, gate27,gate28 : std_logic;

begin

gate1 <= not(a(0) and b(0));
gate2 <=  not(a(1) and b(1));
gate3 <=  not(a(2) and b(2));
gate4 <=  not(a(3) and b(3));
gate5 <=  a(0) and gate1;
gate6 <=  gate1 and b(0);
gate7 <=  gate2 and b(1);
gate8 <= gate3 and b(2);
gate9 <= a(1) and gate2 ;
gate10 <= a(2) and gate3;
gate11 <=  gate4 and b(3);
gate12 <= a(3) and gate4;
gate13 <= b(3) and gate4;
gate14 <= not (gate10 or gate8);
gate15 <= not (gate9 or gate7);
gate16 <= not (gate12 or gate11);
gate17 <= not (gate5 or gate6);
gate18 <= b(1) and gate2 and gate16 and gate14;
gate19 <= gate16 and gate3 and b(2);
gate20 <=  gate17 and gate15 and gate14 and aebin and gate16;
gate21 <=  gate14 and gate15 and gate17 and gate16 and aebin ;
gate22 <=  gate1 and gate16 and gate14 and b(0) and gate15 ;
gate23 <=  gate14 and gate15 and gate17 and gate16 and albin ;
gate24 <=  gate14 and gate16 and gate1 and gate15 and a(0);
gate25 <=  gate16 and gate3 and a(2);
gate26 <=  gate4 and a(3);
gate27 <=  gate17 and gate14 and gate15 and agbin and gate16 ;
gate28 <=  a(1) and gate2 and gate14 and gate16;
albout <=  not (gate24 or gate28 or gate25 or gate26 or gate27 or gate20);
aebout <=  gate14 and aebin and gate15 and gate16 and gate17;
agbout <=  not (gate18 or gate22 or gate23 or gate21 or gate19 or gate13);


end architecture work;
