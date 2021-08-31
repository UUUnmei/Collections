-- ALU

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

entity alu is
port(clk : in std_logic;
		iadd, isub, iand, ior, ixor, inot, ishl, ishr, isar, icmp : in std_logic;		-- 取哪个运算的结果，低有效
	   ealu : in std_logic;  				-- 向总线输出， 低有效
	  A, B : in std_logic_vector(15 downto 0);
	  res: out std_logic_vector(15 downto 0);
	  SF, ZF, OF1, CF : out std_logic);
	  --key_in : in std_logic;
--	  	  dig : out std_logic_vector(3 downto 0);
--	  seg : out std_logic_vector(7 downto 0));
end alu;

architecture work of alu is

component RegF is 
port(clk : in std_logic;
		iadd, isub, iand, ior, ixor: in std_logic;	 -- 低有效
		cout_add, cout_sub, A_H, B_H : in std_logic;  -- AH,BH是操作数AB的最高位
		res_tmp : in std_logic_vector(15 downto 0);  -- res_tmp 接alu的res_tmp
		SF, ZF, OF1, CF : out std_logic);
end component;

component LAadder16 is 
port(A, B: in std_logic_vector(15 downto 0);
	  Cin: in std_logic;	
	  S : out std_logic_vector(15	 downto 0);
	  Cout: out std_logic);
end component;

component LAsuber16 is
port(A, B: in std_logic_vector(15 downto 0);		-- A - B
	  S : out std_logic_vector(15	 downto 0);
	  Cout: out std_logic);							-- Cout 为0表示有借位
end component;

component cal_and is
port(A, B: in std_logic_vector(15 downto 0);		-- A and B
	  S : out std_logic_vector(15	 downto 0));			
end component;

component cal_or is
port(A, B: in std_logic_vector(15 downto 0);		-- A or B
	  S : out std_logic_vector(15	 downto 0));			
end component;

component cal_xor is
port(A, B: in std_logic_vector(15 downto 0);		-- A xor B
	  S : out std_logic_vector(15	 downto 0));			
end component;

component cal_not is
port(A: in std_logic_vector(15 downto 0);		-- A not B
	  S : out std_logic_vector(15	 downto 0));			
end component;

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

component cal_shl is
port(A, B: in std_logic_vector(15 downto 0);		-- A shl B
	  S : out std_logic_vector(15	 downto 0));			
end component;

component cal_shr is
port(A, B: in std_logic_vector(15 downto 0);		-- A shr B
	  S : out std_logic_vector(15	 downto 0));			
end component;

component cal_sar is
port(A, B: in std_logic_vector(15 downto 0);		-- A sar B
	  S : out std_logic_vector(15	 downto 0));			
end component;

signal cin, cout_add, cout_sub, ialu : std_logic := '0';
signal r1 : std_logic_vector(15 downto 0) := (others => '0');
signal tmp, res_add, res_sub, res_and, res_or, res_xor, res_not : std_logic_vector(15 downto 0) := (others => '0');
signal res_shl, res_shr, res_sar : std_logic_vector(15 downto 0) := (others => '0');
signal in_enable : std_logic := '1';
signal ealun : std_logic := '1';
--
--signal clk_t : std_logic;
--signal r_dig : std_logic_vector(3 downto 0) := (others => '1');
--signal r_seg : std_logic_vector(7 downto 0) := (others => '1');
--component SMG is
--port(data: in std_logic_vector(15 downto 0); -- 显示内容
--		clk: in std_logic;
--	  dig : out std_logic_vector(3 downto 0);
--	  seg : out std_logic_vector(7 downto 0));
--end component;

--signal iadd, isub, iand, ior, ixor, inot, ishl, ishr, isar, icmp :  std_logic;
--signal kin,kout,ff : std_logic;
--signal cmd : std_logic_vector(9 downto 0) := "0111111111";
--signal	   ealu :  std_logic:= '1';  				-- 向总线输出， 低有效
--signal	  A : std_logic_vector(15 downto 0) := "1000000000001111";
--signal  B :  std_logic_vector(15 downto 0) := "0000000000000010";
--signal	  res:  std_logic_vector(15 downto 0);
--signal	  SF, ZF, OF1, CF :  std_logic;
begin 
--	ss :	SMG port map(r1, clk, r_dig, r_seg);
--	dig <= r_dig;
--	seg <= r_seg;
--	
--	iadd <= cmd(9);   -- 8011
--	isub <= cmd(8);   -- 800d
--	iand <= cmd(7);   -- 0002
--	ior  <= cmd(6);   -- 800f
--	ixor <= cmd(5);   -- 800d
--	inot <= cmd(4);   -- 7ff0
--	ishl <= cmd(3);   -- 003c
--	ishr <= cmd(2);   -- 2003
--	isar <= cmd(1);   -- e003
--	icmp <= cmd(0);
--	
--	process(clk)
--	variable cnt : integer range 0 to 50000000 := 0;
--	begin
--	if(rising_edge(clk)) then
--		if(cnt = 50000000) then					-- 分频系数5
--			cnt := 0;
--			cmd <= cmd(0) & cmd(9 downto 1);
--		else
--			cnt := cnt + 1;
--		end if;
--	end if;
--	end process;
	
	
	
	F : RegF port map(clk, iadd, isub, iand, ior, ixor, cout_add, cout_sub,
			A(15), B(15), tmp, SF, ZF, OF1, CF);	
	
	adder : LAadder16 port map(A, B, cin, res_add, cout_add);
	suber : LAsuber16 port map(A, B, res_sub, cout_sub);
	and_t : cal_and port map(A, B, res_and);
	or_t  : cal_or port map(A, B, res_or);
	xor_t : cal_xor port map(A, B, res_xor);
	not_t : cal_not port map(A, res_not);
	shl_t : cal_shl port map(A, B, res_shl);
	shr_t : cal_shr port map(A, B, res_shr);
	sar_t : cal_sar port map(A, B, res_sar);
	ialu <= iadd and isub and iand and ior and ixor and inot and ishl and ishr and isar;
	tmp <= res_add when iadd = '0' and isub = '1' and iand = '1' and ior = '1' and ixor = '1' and inot = '1' and ishl = '1' and ishr = '1' and isar = '1'  else
			 res_sub when iadd = '1' and isub = '0' and iand = '1' and ior = '1' and ixor = '1' and inot = '1' and ishl = '1' and ishr = '1' and isar = '1'   else
			 res_and when iadd = '1' and isub = '1' and iand = '0' and ior = '1' and ixor = '1' and inot = '1' and ishl = '1' and ishr = '1' and isar = '1'   else
			 res_or  when iadd = '1' and isub = '1' and iand = '1' and ior = '0' and ixor = '1' and inot = '1' and ishl = '1' and ishr = '1' and isar = '1'   else
			 res_xor when iadd = '1' and isub = '1' and iand = '1' and ior = '1' and ixor = '0' and inot = '1' and ishl = '1' and ishr = '1' and isar = '1'   else
			 res_not when iadd = '1' and isub = '1' and iand = '1' and ior = '1' and ixor = '1' and inot = '0' and ishl = '1' and ishr = '1' and isar = '1'   else
			 res_shl when iadd = '1' and isub = '1' and iand = '1' and ior = '1' and ixor = '1' and inot = '1' and ishl = '0' and ishr = '1' and isar = '1'   else
			 res_shr when iadd = '1' and isub = '1' and iand = '1' and ior = '1' and ixor = '1' and inot = '1' and ishl = '1' and ishr = '0' and isar = '1'   else 
			 res_sar when iadd = '1' and isub = '1' and iand = '1' and ior = '1' and ixor = '1' and inot = '1' and ishl = '1' and ishr = '1' and isar = '0'   else
			 tmp;
			 
	in_enable <= ialu and not icmp;
	ealun <= not ealu;
	u1 : LS377 port map(in_enable, clk, tmp, r1);
	u2 : LS244 port map(ealun, ealun, r1, res);
	
end work;