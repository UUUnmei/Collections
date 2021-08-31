-- 八D锁存器 LS373
-- 功能表见书P250表10-11
-- 测试见waveform21.vwf

library ieee;
use ieee.std_logic_1164.all;

entity LS373 is
port(oc, en: in std_logic;  -- oc 输出控制低有效，=1时输出高阻态；en使能，=1可写入
		d: in std_logic_vector (7 downto 0);
		q: out std_logic_vector (7 downto 0));
end LS373;

architecture work of LS373 is

signal buf : std_logic_vector(7 downto 0) := (others => '0');

begin 
	
	buf <= d when oc = '0' and en = '1' else buf;
--   buf <= d when en = '1' else buf;
	q <= buf when oc = '0' else (others => 'Z');
	
end work;