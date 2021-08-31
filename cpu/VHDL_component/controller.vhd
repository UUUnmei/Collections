library ieee;
use ieee.std_logic_1164.all;


entity controller is
port (-- 节拍信号输入
		T : in std_logic_vector(7 downto 0);
		-- jmp指令要给pc的内容
		to_pc : in std_logic_vector(7 downto 0);
		-- 指令信息输入，详见ir
		dest_operand : in std_logic;  
		source_operand : in std_logic_vector(1 downto 0);
		movri, movrr, addri, addrr, halt, subri, subrr : in std_logic;
		andrr, orrr, xorrr, notrr, shlrr, shrrr, sarrr : in std_logic;
		cmprr, jmprr, jnzrr  : in std_logic;
		
		SF, ZF, OF1, CF : in std_logic;
		
		-- 具体控制信号输出
		imar, ipc, idr, edr, iir, iadd, isub, iand, ior, ixor, inot, ishl, ishr, isar, icmp, ijmp, ealu : out std_logic;
		ia : out std_logic_vector(1 downto 0);  -- 哪个寄存器要从内部数据总线读
		ea : out std_logic_vector(1 downto 0);	-- 哪个寄存器要向内部数据总线写
		addr_dest : out std_logic;				 -- 给选择器的地址
		addr_source : out std_logic_vector(1 downto 0);
		
		out_to_pc : out std_logic_vector(7 downto 0)  -- jmp指令要给pc的内容
		);
end controller;


architecture work of controller is

signal reg_read: std_logic := '1';
signal reg_write: std_logic := '0';

signal r_ia : std_logic_vector(1 downto 0) := "11";

begin

	out_to_pc <= to_pc;

	addr_source <= source_operand;
	addr_dest <= dest_operand;
	
	imar <= not(T(0) or (T(3) and movri) or (T(3) and addri) or (T(3) and subri) 
							or (T(3) and jmprr) or (T(3) and jnzrr));
	ipc <= T(2) or (T(5) and movri) or (T(5) and addri) or (T(5) and subri);
	idr <= T(1) or (T(4) and movri) or (T(4) and addri) or (T(4) and subri) 
						or (T(4) and jmprr) or (T(4) and jnzrr); 
	edr <= (T(3) and movrr) or (T(6) and addri) or (T(4) and addrr)
									or (T(6) and subri) or (T(4) and subrr)
									or (T(4) and andrr) or (T(4) and orrr)
									or (T(4) and xorrr) or (T(4) and notrr)
									or (T(4) and shlrr) or (T(4) and shrrr) or (T(4) and sarrr);
								--	or T(0) or T(1);
	iir <= not T(2);
	iadd <= not ((T(5) and addri) or (T(3) and addrr));
	isub <= not ((T(5) and subri) or (T(3) and subrr) or (T(3) and cmprr));
	iand <= not (T(3) and andrr);
	ior <= not (T(3) and orrr);
	ixor <= not (T(3) and xorrr);
	inot <= not (T(3) and notrr);
	ishl <= not (T(3) and shlrr);
	ishr <= not (T(3) and shrrr);
	isar <= not (T(3) and sarrr);
	icmp <= not (T(3) and cmprr);
	ijmp <= not ((T(5) and jmprr) or (T(5) and jnzrr and not ZF));

	ealu <=not( not ((T(6) and addri) or (T(4) and addrr)
						or (T(6) and subri) or (T(4) and subrr) 
						or (T(4) and andrr) or (T(4) and orrr)
						or (T(4) and xorrr) or (T(4) and notrr)
						or (T(4) and shlrr) or (T(4) and shrrr) or (T(4) and sarrr)) );
	 
	reg_read <= not ((T(5) and movri) or (T(3) and movrr) 
							or (T(6) and addri) or (T(4) and addrr)
							or (T(6) and subri) or (T(4) and subrr)
							or (T(4) and andrr) or (T(4) and orrr)
							or (T(4) and xorrr) or (T(4) and notrr)
							or (T(4) and shlrr) or (T(4) and shrrr) or (T(4) and sarrr));
	reg_write <= T(3) and movrr;
	
--	ia(0) <= reg_read when dest_operand = '0' else '1';
--	ia(1) <= reg_read when dest_operand = '1' else '1';
--	ea(0) <= reg_write when source_operand = "00" else '0';
--	ea(1) <= reg_write when source_operand = "01" else '0';

	ia <= r_ia;
	r_ia(0) <= reg_read or dest_operand;
	r_ia(1) <= reg_read or not dest_operand;
	ea(0) <= reg_write and not source_operand(0) and not source_operand(1);
	ea(1) <= reg_write and source_operand(0) and not source_operand(1);

	
	


end architecture work;