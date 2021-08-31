library ieee;
use ieee.std_logic_1164.all;

-- 指令寄存器和译码器

entity ir is
port(clk, iir : in std_logic;	  -- iir 使能低有效		
		d : in std_logic_vector(15 downto 0);
		
		-- 翻译出的信息：
		-- 操作数
		dest_operand : out std_logic;  -- 0 表示寄存器1， 1表示寄存器2
		source_operand : out std_logic_vector(1 downto 0); -- 00 表示寄存器1， 01 表示寄存器2， 11 表示立即数;	
		-- 指令
		movri, movrr, addri, addrr, halt, subri, subrr  : out std_logic;
		andrr, orrr, xorrr, notrr, shlrr, shrrr, sarrr : out std_logic;
		cmprr, jmprr, jnzrr : out std_logic;
		
		out_to_pc : out std_logic_vector(7 downto 0)  -- jmp指令要给pc的内容
		);
end ir;


architecture work of ir is

component LS377 is 
port(en, clk : in std_logic;		-- en 使能低有效
		d : in std_logic_vector(15 downto 0);
		q : out std_logic_vector(15 downto 0));
end component;

signal buf : std_logic_vector(15 downto 0) := (others => '0');

signal halt_t : std_logic := '0';
signal r_dest_operand : std_logic := '0';
signal r_source_operand : std_logic_vector(1 downto 0) := "00";


begin
	dest_operand <= r_dest_operand;
	source_operand <= r_source_operand;
	halt <= halt_t;
	
	
	u1 : LS377 port map(iir, clk, d, buf);
	
	process(buf)
	begin
	if iir = '0' then
	
	-- 判断指令
	case buf is
		-- HALT
		when x"00FF" => halt_t <= '1'; movri <= '0'; movrr <= '0';
							 addri <= '0'; addrr <= '0';
							 subri <= '0'; subrr <= '0';
							 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0'; 
							 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
							 cmprr <= '0'; jmprr <= '0'; jnzrr <= '0';
		-- MOVRI
		when x"100F" 
			| x"101F" =>
							 halt_t <= '0'; movri <= '1'; movrr <= '0';
							 addri <= '0'; addrr <= '0';
							 subri <= '0'; subrr <= '0';
							 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0';
							 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
							 cmprr <= '0'; jmprr <= '0'; jnzrr <= '0';
		-- MOVRR
		when x"1100"  
			| x"1101" 
			| x"1110" 
			| x"1111" =>
							 halt_t <= '0'; movri <= '0'; movrr <= '1';
							 addri <= '0'; addrr <= '0';
							 subri <= '0'; subrr <= '0';
							 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0';
							 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
							 cmprr <= '0'; jmprr <= '0'; jnzrr <= '0';
		-- ADDRI
		when x"200F" 
			| x"201F" =>	
							 halt_t <= '0'; movri <= '0'; movrr <= '0';
							 addri <= '1'; addrr <= '0';
							 subri <= '0'; subrr <= '0';
							 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0';
							 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
							 cmprr <= '0'; jmprr <= '0'; jnzrr <= '0';
		-- ADDRR
		when x"2100" 
			| x"2101"
			| x"2110" 
			| x"2111" => 
							 halt_t <= '0'; movri <= '0'; movrr <= '0';
							 addri <= '0'; addrr <= '1';
							 subri <= '0'; subrr <= '0';
							 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0';
							 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
							 cmprr <= '0'; jmprr <= '0'; jnzrr <= '0';
		-- SUBRI
		when x"220F" 
			| x"221F" =>	
							 halt_t <= '0'; movri <= '0'; movrr <= '0';
							 addri <= '0'; addrr <= '0';
							 subri <= '1'; subrr <= '0';
							 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0';
							 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
							 cmprr <= '0'; jmprr <= '0'; jnzrr <= '0';
		-- SUBRR
		when x"2300" 
			| x"2301"
			| x"2310" 
			| x"2311" => 
							 halt_t <= '0'; movri <= '0'; movrr <= '0';
							 addri <= '0'; addrr <= '0';
							 subri <= '0'; subrr <= '1';
							 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0';
							 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
							 cmprr <= '0'; jmprr <= '0'; jnzrr <= '0';
		-- AND
		when x"3000" 
			| x"3001"
			| x"3010" 
			| x"3011" => 
							 halt_t <= '0'; movri <= '0'; movrr <= '0';
							 addri <= '0'; addrr <= '0';
							 subri <= '0'; subrr <= '0';
							 andrr <= '1'; orrr <= '0'; xorrr <= '0'; notrr <= '0';
							 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
							 cmprr <= '0'; jmprr <= '0'; jnzrr <= '0';
		-- OR
		when x"3100" 
			| x"3101"
			| x"3110" 
			| x"3111" => 
							 halt_t <= '0'; movri <= '0'; movrr <= '0';
							 addri <= '0'; addrr <= '0';
							 subri <= '0'; subrr <= '0';
							 andrr <= '0'; orrr <= '1'; xorrr <= '0'; notrr <= '0';
							 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
							 cmprr <= '0'; jmprr <= '0'; jnzrr <= '0';
		
		-- XOR
		when x"3200" 
			| x"3201"
			| x"3210" 
			| x"3211" => 
							 halt_t <= '0'; movri <= '0'; movrr <= '0';
							 addri <= '0'; addrr <= '0';
							 subri <= '0'; subrr <= '0';
							 andrr <= '0'; orrr <= '0'; xorrr <= '1'; notrr <= '0';
							 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
							 cmprr <= '0'; jmprr <= '0'; jnzrr <= '0';
		
		-- NOT
		when x"330F" 
			| x"331F" => 
							 halt_t <= '0'; movri <= '0'; movrr <= '0';
							 addri <= '0'; addrr <= '0';
							 subri <= '0'; subrr <= '0';
							 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '1';
							 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
							 cmprr <= '0'; jmprr <= '0'; jnzrr <= '0';
		-- SHL					 
		when x"3400" 
			| x"3401"
			| x"3410" 
			| x"3411" => 
							 halt_t <= '0'; movri <= '0'; movrr <= '0';
							 addri <= '0'; addrr <= '0';
							 subri <= '0'; subrr <= '0';
							 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0';
							 shlrr <= '1'; shrrr <= '0'; sarrr <= '0';
							 cmprr <= '0'; jmprr <= '0'; jnzrr <= '0';
		-- SHR
		when x"3500" 
			| x"3501"
			| x"3510" 
			| x"3511" => 
							 halt_t <= '0'; movri <= '0'; movrr <= '0';
							 addri <= '0'; addrr <= '0';
							 subri <= '0'; subrr <= '0';
							 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0';
							 shlrr <= '0'; shrrr <= '1'; sarrr <= '0';
							 cmprr <= '0'; jmprr <= '0'; jnzrr <= '0';
		-- SAR					 
		when x"3600" 
			| x"3601"
			| x"3610" 
			| x"3611" => 
							 halt_t <= '0'; movri <= '0'; movrr <= '0';
							 addri <= '0'; addrr <= '0';
							 subri <= '0'; subrr <= '0';
							 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0';
							 shlrr <= '0'; shrrr <= '0'; sarrr <= '1';
							 cmprr <= '0'; jmprr <= '0'; jnzrr <= '0';
		
		-- CMP
		when x"3700" 
			| x"3701"
			| x"3710" 
			| x"3711" => 
							 halt_t <= '0'; movri <= '0'; movrr <= '0';
							 addri <= '0'; addrr <= '0';
							 subri <= '0'; subrr <= '0';
							 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0';
							 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
							 cmprr <= '1'; jmprr <= '0'; jnzrr <= '0';
							 
	   -- JMP
		-- x"4000" to x"40FF" 但是报错 Error (10319): VHDL aggregate error : choice must be discrete
		-- 指令改为40FF
		when x"40FF" =>
							 halt_t <= '0'; movri <= '0'; movrr <= '0';
							 addri <= '0'; addrr <= '0';
							 subri <= '0'; subrr <= '0';
							 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0';
							 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
							 cmprr <= '0'; jmprr <= '1'; jnzrr <= '0';
		
		-- JNZ					 
		when x"41FF" =>
							 halt_t <= '0'; movri <= '0'; movrr <= '0';
							 addri <= '0'; addrr <= '0';
							 subri <= '0'; subrr <= '0';
							 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0';
							 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
							 cmprr <= '0'; jmprr <= '0'; jnzrr <= '1';
							 
							 
		-- 指令非法就停机
		when others =>  halt_t <= '1'; movri <= '0'; movrr <= '0';
							 addri <= '0'; addrr <= '0';
							 subri <= '0'; subrr <= '0';
							 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0';
							 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
							 cmprr <= '0'; jmprr <= '0'; jnzrr <= '0';
							 
		
	end case;
	
	-- 判断操作数
	-- 这个长度和寄存器和alu之间那个选择器有关
	r_dest_operand <= buf(4);
	r_source_operand <= buf(1 downto 0);
	
	end if;
	end process;
	
	out_to_pc <= d(7 downto 0);


end architecture work;