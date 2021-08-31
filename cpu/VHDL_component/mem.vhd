-- testbench

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;

entity mem is
port(
	cs, wr : in std_logic;  -- cs使能端低有效，wr读写控制，=1是读，=0是写
	addr : in std_logic_vector(7 downto 0);
	--mem_in : in std_logic_vector(15 downto 0);
	mem_out : out std_logic_vector(15 downto 0));
end mem;

architecture work of mem is

type rom is array(0 to 255) of std_logic_vector(7 downto 0);

signal memory : rom := (
"00010000",
"00001111",
"00000000",
"00000110",
"00100000",
"00001111",
"00000000",
"00000111",
"00010001",
"00010000",
"00100001",
"00000001",
"00100010",
"00001111",
"11111111",
"11111101",
"00100011",
"00000001",
"00110010",
"00010001",
"00010000",
"00011111",
"01000000",
"00000011",
"00110000",
"00000001",
"00110011",
"00011111",
"00110001",
"00000001",
"00010000",
"00011111",
"00000000",
"00000010",
"00110110",
"00000001",
"00110100",
"00000001",
"00010001",
"00000000",
"00110101",
"00000001",
"00110111",
"00000001",
"01000001",
"11111111",
"00000000",
"00010100",
"00000000",
"11111111",
others => "00000000"
);

--movri r0, 6
--addri r0, 7
--movrr r1, r0
--addrr r0, r1
--subri r0, -3
--subrr r0, r1
--xor r1, r1
--movri r1, 16387
--and r0, r1
--not r1
--or r0, r1
--movri r1, 2
--sar r0, r1
--shl r0, r1
--movrr r0,r0
--shr r0, r1
--cmp r0, r1
--jnz 20
--halt

constant one : std_logic_vector(7 downto 0) := "00000001";
signal r_mem_out : std_logic_vector(15 downto 0) := (others => '0');

begin

	
	--！！！ vwf一仿真这就出问题，modelsim就好使。。
	process(cs, wr, addr)
	begin
	if(cs = '0')  then
		if(wr = '1') then
			mem_out(15 downto 8) <= memory(conv_integer(addr));			-- 偶地址
			mem_out(7 downto 0)  <= memory(conv_integer(addr or one));  -- 奇地址
		end if;
	end if;
	end process;


end architecture work;