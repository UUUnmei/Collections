

library ieee;
use ieee.std_logic_1164.all;

entity gen_pulse is
port(clk, clr : in std_logic;			-- 清零低有效
		T : out std_logic_vector(7 downto 0));
--		-- 单独上板子
--	  	  dig : out std_logic_vector(3 downto 0);
--	  seg : out std_logic_vector(7 downto 0));
end gen_pulse;


architecture work of gen_pulse is

component DFF74 is
port(D, prn, clrn, clk:in std_logic;
		Q, Qn: out std_logic);
end component;


signal buf, nn : std_logic_vector(7 downto 0) := (others => '0');
signal clkn : std_logic ;



-- 单独上板子
--
--signal clk_t : std_logic := '0';
--signal r_dig : std_logic_vector(3 downto 0) := (others => '1');
--signal r_seg : std_logic_vector(7 downto 0) := (others => '1');
--component SMG is
--port(data: in std_logic_vector(15 downto 0); -- 显示内容
--		clk, en: in std_logic;
--	  dig : out std_logic_vector(3 downto 0);
--	  seg : out std_logic_vector(7 downto 0));
--end component;
begin

--	clkn <= not clk_t;
--	process(clk)
--	variable cnt : integer range 0 to 12500000 := 0;
--	begin
--	if(rising_edge(clk)) then
--		if(cnt = 12500000) then				
--			cnt := 0;
--			clk_t <= not clk_t;
--		else
--			cnt := cnt + 1;
--		end if;
--	end if;
--	end process;
--	ss :	SMG port map("00000000" & buf, clk, '0', r_dig, r_seg);
--	dig <= r_dig;
--	seg <= r_seg;
	

	clkn <= not clk;
	u0 : DFF74 port map(buf(7), clr, '1', clkn,  buf(0), nn(0));
	u1 : DFF74 port map(buf(0), '1', clr, clkn,  buf(1), nn(1));
	u2 : DFF74 port map(buf(1), '1', clr, clkn,  buf(2), nn(2));
	u3 : DFF74 port map(buf(2), '1', clr, clkn,  buf(3), nn(3));
	u4 : DFF74 port map(buf(3), '1', clr, clkn,  buf(4), nn(4));
	u5 : DFF74 port map(buf(4), '1', clr, clkn,  buf(5), nn(5));
	u6 : DFF74 port map(buf(5), '1', clr, clkn,  buf(6), nn(6));
	u7 : DFF74 port map(buf(6), '1', clr, clkn,  buf(7), nn(7));
	
	T <= buf;

end architecture work;