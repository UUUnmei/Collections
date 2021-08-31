-- 整体
library ieee;
use ieee.std_logic_1164.all;

entity sys is
port(clk, clr : in std_logic;
		-- 显示输出
	  dig : out std_logic_vector(3 downto 0);
	  seg : out std_logic_vector(7 downto 0);
	  ia2 : out std_logic_vector(1 downto 0);
	  halt2 : out std_logic;
----		
----  逻辑分析仪

		bus_out : out std_logic_vector(15 downto 0);
		TT : out std_logic_vector(7 downto 0);
		imar2, ipc2, idr2, iir2 ,clk2: out std_logic; 
		ealu2 ,edr2: out std_logic;
   	ea2 : out std_logic_vector(1 downto 0);
		movri2, movrr2, addri2, addrr2, subri2, subrr2  : out std_logic;
		andrr2, orrr2, xorrr2, notrr2 : out std_logic

		-- 仿真
--		bus_out : out std_logic_vector(15 downto 0);
--		TT : out std_logic_vector(7 downto 0);
--		imar2, ipc2, idr2, iir2 ,clk2, halt2, clk2: out std_logic; 
--		ealu2 ,edr2: out std_logic;
--		ia2 : out std_logic_vector(1 downto 0);
--   	ea2 : out std_logic_vector(1 downto 0);
--		movri2, movrr2, addri2, addrr2, subri2, subrr2  : out std_logic;
--		r1_to_alu2, r2_to_alu2: out std_logic_vector(15 downto 0);
--		SF, ZF, OF1, CF : out std_logic
);
end sys;


architecture work of sys is

-- 数码管显示
component SMG is
port(data: in std_logic_vector(15 downto 0); -- 显示内容
		clk, en: in std_logic;
	  dig : out std_logic_vector(3 downto 0);
	  seg : out std_logic_vector(7 downto 0));
end component;
signal r_dig : std_logic_vector(3 downto 0) := (others => '1');
signal r_seg : std_logic_vector(7 downto 0) := (others => '1');

-- 16位二选一
component c21_16 is
port(en: in std_logic;  --en地址线 为0选d0 为1选d1
		d0, d1: in std_logic_vector (15 downto 0);
		q: out std_logic_vector (15 downto 0));
end component;

-- 16位四选一
component c41_16 is
port(en: in std_logic_vector(1 downto 0);  --en地址线
		d0, d1, d2, d3: in std_logic_vector (15 downto 0);
		q: out std_logic_vector (15 downto 0));
end component;


-- 程序计数器
component pc is
port(
	clr, clk, ipc: in std_logic;		--clr低有效， ipc使能高有效 
	ijmp : in std_logic;
	target : in std_logic_vector(7 downto 0); -- 跳转指令的输入
	pc_out : out std_logic_vector(7 downto 0));
end component;

-- 地址寄存器
component mar is
port(
	imar, clk : in std_logic ; -- imar使能低有效
	mar_in : in std_logic_vector(7 downto 0);
	mar_out : out std_logic_vector(7 downto 0));
end component;

-- 存储器
component mem is
port(
	cs, wr : in std_logic;  -- cs使能端低有效，wr读写控制，=1是读，=0是写
	addr : in std_logic_vector(7 downto 0);
	--mem_in : in std_logic_vector(15 downto 0);
	mem_out : out std_logic_vector(15 downto 0));
end component;

-- 数据寄存器
component dr is
port(edr, idr, clk : in std_logic;
	  dr_in : in std_logic_vector(15 downto 0);
	  dr_out : out std_logic_vector(15 downto 0));
end component;

-- 时钟信号源
component gen_clk is
port(clk_in, en : in std_logic;			-- en低有效
		clk_out : out std_logic);
end component;

-- 节拍发生器
component gen_pulse is
port(clk, clr : in std_logic;			-- 清零低有效
		T : out std_logic_vector(7 downto 0));
end component;

-- 操作控制器
component controller is
port (-- 节拍信号输入
		T : in std_logic_vector(7 downto 0);
		to_pc : in std_logic_vector(7 downto 0);
		-- 指令信息输入，详见ir
		dest_operand : in std_logic;  
		source_operand : in std_logic_vector(1 downto 0);
		movri, movrr, addri, addrr, halt, subri, subrr: in std_logic;
		andrr, orrr, xorrr, notrr, shlrr, shrrr, sarrr, cmprr, jmprr, jnzrr  : in std_logic;
		
		SF, ZF, OF1, CF : in std_logic;
		-- 具体控制信号输出
		imar, ipc, idr, edr, iir, iadd, isub, iand, ior, ixor, inot, ishl, ishr, isar, icmp, ijmp, ealu : out std_logic;
		ia : out std_logic_vector(1 downto 0);  -- 哪个寄存器要从内部数据总线读
		ea : out std_logic_vector(1 downto 0);	-- 哪个寄存器要向内部数据总线写
		addr_dest : out std_logic;				 -- 给选择器的地址
		addr_source : out std_logic_vector(1 downto 0);
		
		out_to_pc : out std_logic_vector(7 downto 0)
		);
end component;

-- 指令寄存器和译码器
component ir is
port(clk, iir : in std_logic;	  -- iir 使能低有效		
		d : in std_logic_vector(15 downto 0);
		
		-- 翻译出的信息：
		-- 操作数
		dest_operand : out std_logic;  -- 0 表示寄存器1， 1表示寄存器2
		source_operand : out std_logic_vector(1 downto 0); -- 00 表示寄存器1， 01 表示寄存器2， 11 表示立即数;	
		-- 指令
		movri, movrr, addri, addrr, halt, subri, subrr : out std_logic;
		andrr, orrr, xorrr, notrr,  shlrr, shrrr, sarrr, cmprr, jmprr, jnzrr : out std_logic;
		out_to_pc : out std_logic_vector(7 downto 0)
		);
end component;

-- 寄存器（累加器）
component Reg is 
port(clk, in_from_dbus, out_to_dbus: in std_logic;
			-- out_to_alu
		d : in std_logic_vector(15 downto 0);
		qalu, qbus : out std_logic_vector(15 downto 0));
end component;

-- 算术逻辑单元ALU
component alu is
port(clk : in std_logic;
		iadd, isub, iand, ior, ixor, inot, ishl, ishr, isar, icmp  : in std_logic;		-- 取哪个运算的结果，低有效
	   ealu : in std_logic;  				-- 向总线输出， 低有效
	  A, B : in std_logic_vector(15 downto 0);
	  res: out std_logic_vector(15 downto 0);
	  SF, ZF, OF1, CF : out std_logic);
end component;

-- 总线
component dbus is
  port(ealu,edr: in std_logic;
			ea : in std_logic_vector(1 downto 0);
			alu_to_dbus,dr_out,r1_to_dbus,r2_to_dbus:in std_logic_vector(15 downto 0);
			cout : out std_logic_vector(15 downto 0)
      );
end component;

signal clk_t : std_logic := '0';
signal T : std_logic_vector(7 downto 0) := (others => '0');
signal idr,edr, ipc : std_logic := '0';
signal  imar, iir, iadd, isub, iand, ior, ixor, inot, ishl, ishr, isar , icmp , ijmp :std_logic := '1';
signal ealu : std_logic := '0';
signal ia : std_logic_vector(1 downto 0) := "11";  -- 哪个寄存器要从内部数据总线读
signal ea : std_logic_vector(1 downto 0) := "00";	-- 哪个寄存器要向内部数据总线写
signal addr_dest : std_logic := '0';				 -- 给选择器的地址
signal addr_source : std_logic_vector(1 downto 0) := "00";
signal pc_out, mar_out  : std_logic_vector(7 downto 0) := (others => '0');
signal mem_out, dr_out, r1_to_alu, r1_to_dbus, r2_to_alu, r2_to_dbus : std_logic_vector(15 downto 0):= (others => '0');
signal alu_to_dbus, c1,c2, from_bus: std_logic_vector(15 downto 0) := (others => '0') ;
signal dest_operand :  std_logic := '0';  -- 0 表示寄存器1， 1表示寄存器2
signal source_operand : std_logic_vector(1 downto 0) := "00"; -- 00 表示寄存器1， 01 表示寄存器2， 11 表示立即数;	
signal movri, movrr, addri, addrr, halt, subri, subrr, andrr, orrr, xorrr, notrr, cmprr, jmprr, jnzrr : std_logic := '0'; -- 指令
signal shlrr, shrrr, sarrr : std_logic := '0';
signal SF_t, ZF_t, OF1_t, CF_t :  std_logic;
signal jmp_dest1, jmp_dest2 :  std_logic_vector(7 downto 0);

signal  bus_data : std_logic_vector(15 downto 0) := (others => '0');
signal data : std_logic_vector(15 downto 0) := (others => '0');
signal alu_en, dr_en, r1_en, r2_en : std_logic;
signal cs, wr, clken : std_logic;

begin 
--		jmprr2 <= jmprr;
--		
--		SF <= SF_t;		
--		ZF <= ZF_t;
--		OF1 <= OF1_t;
--		CF <= CF_t;
	
--		bus_out <= from_bus;
		bus_out <= bus_data;
		TT <= T;
		imar2 <= imar;
		ipc2 <= ipc;
		idr2 <= idr;
		edr2 <= dr_en;
		iir2 <= iir;
--		pc2 <= pc_out;
		ealu2 <= alu_en;
		halt2  <= halt;
		movri2 <= movri;
		movrr2 <= movrr;
		addri2 <= addri;
		addrr2 <= addrr;
		subri2 <= subri;
		subrr2 <= subrr;
--
		andrr2 <= andrr;
    	orrr2 <= orrr;
		xorrr2 <= xorrr;
		notrr2 <= notrr;
--		iadd2 <= iadd;
--		isub2 <= isub;
--		iand2 <= iand;
--		ior2 <= ior;
--		ixor2 <= ixor;
--  	inot2 <= inot;
		
		
--		mar_out2 <= mar_out;
--		mem_out2 <= mem_out;
--		dr_out2 <= dr_out;
--		r1_to_alu2 <= r1_to_alu;
--		r2_to_alu2 <= r2_to_alu;
		ia2 <= ia;
		ea2(0) <= r1_en;
		ea2(1) <= r2_en;

--		addr_dest2 <= addr_dest;
--		addr_source2 <= addr_source;
		
	

--	
	data <= r1_to_alu when ia = "10" else
			  r2_to_alu when ia = "01" else
			  data;


	--data <= dr_out;
	dig <= r_dig;
	seg <= r_seg;
	
	clk2 <= clk_t;
	alu_en <= clr and ealu;
	dr_en <= edr or not clr;
	r1_en <= ea(0) and clr;
	r2_en <= ea(1) and clr;
	
	k : alu port map(clk_t, iadd, isub, iand, ior, ixor, inot, ishl, ishr, isar, icmp, alu_en, c1, c2, bus_data, SF_t, ZF_t, OF1_t, CF_t);
	f : dr port map(dr_en, idr, clk_t, mem_out, bus_data);
	r1 : Reg port map(clk_t, ia(0), r1_en, bus_data, r1_to_alu, bus_data);
	r2 : Reg port map(clk_t, ia(1), r2_en, bus_data, r2_to_alu, bus_data);
	g : ir port map(clk_t, iir, bus_data, dest_operand, source_operand,
		movri, movrr, addri, addrr, halt, subri, subrr, andrr, orrr, xorrr, notrr, shlrr, shrrr, sarrr, cmprr, jmprr, jnzrr, jmp_dest1);
	j : c41_16 port map(addr_source, r1_to_alu, r2_to_alu, x"0000", bus_data, c2);
	
	ss :	SMG port map(data, clk, halt, r_dig, r_seg);
			  
	clken <= not clr;

	
	a : gen_clk port map(clk, clken, clk_t);
	b : gen_pulse port map(clk_t, clr, T);
	c : pc port map(clr, clk_t, ipc, ijmp, jmp_dest2, pc_out);
	d : mar port map(imar, clk_t, pc_out, mar_out);
	cs <= '0';
	wr <= '1';
--	cs2 <= cs;
--	wr2 <= wr;
	e : mem port map(cs, wr, mar_out, mem_out);
	h : controller port map(T, jmp_dest1, dest_operand, source_operand, movri, movrr, addri, addrr,
		halt, subri, subrr, andrr, orrr, xorrr, notrr,  shlrr, shrrr, sarrr, cmprr, jmprr, jnzrr,
		SF_t, ZF_t, OF1_t, CF_t,
		imar, ipc, idr, edr, iir, iadd, isub, iand, ior, ixor, inot, ishl, ishr, isar, icmp, ijmp,
		ealu, ia, ea, addr_dest, addr_source, jmp_dest2);
	
	i : c21_16 port map(addr_dest, r1_to_alu, r2_to_alu, c1);


end architecture work;