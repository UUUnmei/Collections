library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- 数码管显示

entity SMG is
port(data: in std_logic_vector(15 downto 0);
	  clk, en: in std_logic;
	  dig : out std_logic_vector(3 downto 0);
	  seg : out std_logic_vector(7 downto 0));
end SMG;

architecture work of SMG is
	
signal count: integer := 0;
signal single_beat : integer := 250000;	-- 5 ms
signal r_dig : std_logic_vector(3 downto 0) := "1110" ;
--signal data : std_logic_vector(7 downto 0) := "11110001"; -- F1
begin
	
	-- 5 ms显示最低位，5 ms显示次低位
	process(clk)
	begin
	if(rising_edge(clk)) then
		if(count >= single_beat) then
			count <= 0;
			if(r_dig = "1110") then
				r_dig <= "1101";
			elsif (r_dig = "1101") then
				r_dig <= "1011";
			elsif (r_dig = "1011") then
				r_dig <= "0111";
			else
				r_dig <= "1110";
			end if;
		else	
			count <= count + 1;
		end if;
	end if;
	end process;
	
	dig <= r_dig;
	
	process(clk)
	begin
	if(en = '1') then
		seg <= "11111111";
		
	elsif(rising_edge(clk)) then
		if(r_dig = "1110") then
		-- 最低位
			case data(3 downto 0) is
				when "0000" => seg <= "11000000";
				when "0001" => seg <= "11111001";
				when "0010" => seg <= "10100100";
				when "0011" => seg <= "10110000";
				when "0100" => seg <= "10011001";
				when "0101" => seg <= "10010010";
				when "0110" => seg <= "10000010";
				when "0111" => seg <= "11111000";
				when "1000" => seg <= "10000000";
				when "1001" => seg <= "10010000";
				when "1010" => seg <= "10001000";
				when "1011" => seg <= "10000011";
				when "1100" => seg <= "11000110";
				when "1101" => seg <= "10100001";
				when "1110" => seg <= "10000110";
				when "1111" => seg <= "10001110";
				when others => seg <= "11111111";
			end case;
		elsif(r_dig = "1101") then
			case data(7 downto 4) is
				when "0000" => seg <= "11000000";
				when "0001" => seg <= "11111001";
				when "0010" => seg <= "10100100";
				when "0011" => seg <= "10110000";
				when "0100" => seg <= "10011001";
				when "0101" => seg <= "10010010";
				when "0110" => seg <= "10000010";
				when "0111" => seg <= "11111000";
				when "1000" => seg <= "10000000";
				when "1001" => seg <= "10010000";
				when "1010" => seg <= "10001000";
				when "1011" => seg <= "10000011";
				when "1100" => seg <= "11000110";
				when "1101" => seg <= "10100001";
				when "1110" => seg <= "10000110";
				when "1111" => seg <= "10001110";
				when others => seg <= "11111111";
			end case;
		elsif(r_dig = "1011") then
			case data(11 downto 8) is
				when "0000" => seg <= "11000000";
				when "0001" => seg <= "11111001";
				when "0010" => seg <= "10100100";
				when "0011" => seg <= "10110000";
				when "0100" => seg <= "10011001";
				when "0101" => seg <= "10010010";
				when "0110" => seg <= "10000010";
				when "0111" => seg <= "11111000";
				when "1000" => seg <= "10000000";
				when "1001" => seg <= "10010000";
				when "1010" => seg <= "10001000";
				when "1011" => seg <= "10000011";
				when "1100" => seg <= "11000110";
				when "1101" => seg <= "10100001";
				when "1110" => seg <= "10000110";
				when "1111" => seg <= "10001110";
				when others => seg <= "11111111";
			end case;
		else
			
			case data(15 downto 12) is
				when "0000" => seg <= "11000000";
				when "0001" => seg <= "11111001";
				when "0010" => seg <= "10100100";
				when "0011" => seg <= "10110000";
				when "0100" => seg <= "10011001";
				when "0101" => seg <= "10010010";
				when "0110" => seg <= "10000010";
				when "0111" => seg <= "11111000";
				when "1000" => seg <= "10000000";
				when "1001" => seg <= "10010000";
				when "1010" => seg <= "10001000";
				when "1011" => seg <= "10000011";
				when "1100" => seg <= "11000110";
				when "1101" => seg <= "10100001";
				when "1110" => seg <= "10000110";
				when "1111" => seg <= "10001110";
				when others => seg <= "11111111";
			end case;
		end if;
	end if;
	end process;


end architecture work;