library ieee;
use ieee.std_logic_1164.all;

entity RegF is 
port(clk : in std_logic;
		iadd, isub, iand, ior, ixor: in std_logic;	 -- 低有效
		cout_add, cout_sub, A_H, B_H : in std_logic;  -- AH,BH是操作数AB的最高位
		res_tmp : in std_logic_vector(15 downto 0);  -- res_tmp 接alu的tmp
		SF, ZF, OF1, CF : out std_logic);
end RegF;

architecture work of RegF is

signal SF_t, ZF_t, OF1_t, CF_t : std_logic := '0';

begin

	SF <= SF_t;
	ZF <= ZF_t;
	OF1 <= OF1_t;
	CF <= CF_t;
		
	process(clk)
	begin
	if(rising_edge(clk) and (iadd and isub and iand and ior and ixor) = '0') then
		if(res_tmp = x"0000") then
			ZF_t <= '1';
		else
			ZF_t <= '0';
		end if;

		SF_t <= res_tmp(15);
		
		-- and,or,xor操作时 of cf清0	 not啥也不影响
		-- cout_add 是高有效 cout_sub 是低有效
		-- 暂不考虑移位操作对cf的影响
		CF_t <= ((cout_add and not iadd) or (cout_sub and not isub)) and iand and ior and ixor;
		
		-- 两正数算出负数  
		-- 两负数算出正数
		OF1_t <= ((A_H and B_H and not res_tmp(15)) or (not A_H and not B_H and res_tmp(15)))		
				and iand and ior and ixor and ZF_t;
	end if;
	end process;
	

	
end architecture work;