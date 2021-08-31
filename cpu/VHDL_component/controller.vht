-- Copyright (C) 2018  Intel Corporation. All rights reserved.
-- Your use of Intel Corporation's design tools, logic functions 
-- and other software and tools, and its AMPP partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Intel Program License 
-- Subscription Agreement, the Intel Quartus Prime License Agreement,
-- the Intel FPGA IP License Agreement, or other applicable license
-- agreement, including, without limitation, that your use is for
-- the sole purpose of programming logic devices manufactured by
-- Intel and sold by Intel or its authorized distributors.  Please
-- refer to the applicable agreement for further details.

-- ***************************************************************************
-- This file contains a Vhdl test bench template that is freely editable to   
-- suit user's needs .Comments are provided in each section to help the user  
-- fill out necessary details.                                                
-- ***************************************************************************
-- Generated on "06/29/2021 21:34:03"
                                                            
-- Vhdl Test Bench template for design  :  controller
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                
use ieee.std_logic_unsigned.all;
ENTITY controller_vhd_tst IS
END controller_vhd_tst;
ARCHITECTURE controller_arch OF controller_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL addr_dest : STD_LOGIC;
SIGNAL addr_source : STD_LOGIC_VECTOR(1 DOWNTO 0);
SIGNAL addri : STD_LOGIC;
SIGNAL addrr : STD_LOGIC;
SIGNAL andrr : STD_LOGIC;
SIGNAL CF : STD_LOGIC;
SIGNAL cmprr : STD_LOGIC;
SIGNAL dest_operand : STD_LOGIC;
SIGNAL ea : STD_LOGIC_VECTOR(1 DOWNTO 0);
SIGNAL ealu : STD_LOGIC;
SIGNAL edr : STD_LOGIC;
SIGNAL halt : STD_LOGIC;
SIGNAL ia : STD_LOGIC_VECTOR(1 DOWNTO 0);
SIGNAL iadd : STD_LOGIC;
SIGNAL iand : STD_LOGIC;
SIGNAL icmp : STD_LOGIC;
SIGNAL idr : STD_LOGIC;
SIGNAL iir : STD_LOGIC;
SIGNAL ijmp : STD_LOGIC;
SIGNAL imar : STD_LOGIC;
SIGNAL inot : STD_LOGIC;
SIGNAL ior : STD_LOGIC;
SIGNAL ipc : STD_LOGIC;
SIGNAL isar : STD_LOGIC;
SIGNAL ishl : STD_LOGIC;
SIGNAL ishr : STD_LOGIC;
SIGNAL isub : STD_LOGIC;
SIGNAL ixor : STD_LOGIC;
SIGNAL jmprr : STD_LOGIC;
SIGNAL jzrr : STD_LOGIC;
SIGNAL movri : STD_LOGIC;
SIGNAL movrr : STD_LOGIC;
SIGNAL notrr : STD_LOGIC;
SIGNAL OF1 : STD_LOGIC;
SIGNAL orrr : STD_LOGIC;
SIGNAL out_to_pc : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL sarrr : STD_LOGIC;
SIGNAL SF : STD_LOGIC;
SIGNAL shlrr : STD_LOGIC;
SIGNAL shrrr : STD_LOGIC;
SIGNAL source_operand : STD_LOGIC_VECTOR(1 DOWNTO 0) := "00";
SIGNAL subri : STD_LOGIC;
SIGNAL subrr : STD_LOGIC;
SIGNAL T : STD_LOGIC_VECTOR(7 DOWNTO 0) := "00000001";
SIGNAL to_pc : STD_LOGIC_VECTOR(7 DOWNTO 0) := (others => '0');
SIGNAL xorrr : STD_LOGIC;
SIGNAL ZF : STD_LOGIC;
COMPONENT controller
	PORT (
	addr_dest : OUT STD_LOGIC;
	addr_source : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
	addri : IN STD_LOGIC;
	addrr : IN STD_LOGIC;
	andrr : IN STD_LOGIC;
	CF : IN STD_LOGIC;
	cmprr : IN STD_LOGIC;
	dest_operand : IN STD_LOGIC;
	ea : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
	ealu : OUT STD_LOGIC;
	edr : OUT STD_LOGIC;
	halt : IN STD_LOGIC;
	ia : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
	iadd : OUT STD_LOGIC;
	iand : OUT STD_LOGIC;
	icmp : OUT STD_LOGIC;
	idr : OUT STD_LOGIC;
	iir : OUT STD_LOGIC;
	ijmp : OUT STD_LOGIC;
	imar : OUT STD_LOGIC;
	inot : OUT STD_LOGIC;
	ior : OUT STD_LOGIC;
	ipc : OUT STD_LOGIC;
	isar : OUT STD_LOGIC;
	ishl : OUT STD_LOGIC;
	ishr : OUT STD_LOGIC;
	isub : OUT STD_LOGIC;
	ixor : OUT STD_LOGIC;
	jmprr : IN STD_LOGIC;
	jzrr : IN STD_LOGIC;
	movri : IN STD_LOGIC;
	movrr : IN STD_LOGIC;
	notrr : IN STD_LOGIC;
	OF1 : IN STD_LOGIC;
	orrr : IN STD_LOGIC;
	out_to_pc : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
	sarrr : IN STD_LOGIC;
	SF : IN STD_LOGIC;
	shlrr : IN STD_LOGIC;
	shrrr : IN STD_LOGIC;
	source_operand : IN STD_LOGIC_VECTOR(1 DOWNTO 0) ;
	subri : IN STD_LOGIC;
	subrr : IN STD_LOGIC;
	T : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
	to_pc : IN STD_LOGIC_VECTOR(7 DOWNTO 0) ;
	xorrr : IN STD_LOGIC;
	ZF : IN STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : controller
	PORT MAP (
-- list connections between master ports and signals
	addr_dest => addr_dest,
	addr_source => addr_source,
	addri => addri,
	addrr => addrr,
	andrr => andrr,
	CF => CF,
	cmprr => cmprr,
	dest_operand => dest_operand,
	ea => ea,
	ealu => ealu,
	edr => edr,
	halt => halt,
	ia => ia,
	iadd => iadd,
	iand => iand,
	icmp => icmp,
	idr => idr,
	iir => iir,
	ijmp => ijmp,
	imar => imar,
	inot => inot,
	ior => ior,
	ipc => ipc,
	isar => isar,
	ishl => ishl,
	ishr => ishr,
	isub => isub,
	ixor => ixor,
	jmprr => jmprr,
	jzrr => jzrr,
	movri => movri,
	movrr => movrr,
	notrr => notrr,
	OF1 => OF1,
	orrr => orrr,
	out_to_pc => out_to_pc,
	sarrr => sarrr,
	SF => SF,
	shlrr => shlrr,
	shrrr => shrrr,
	source_operand => source_operand,
	subri => subri,
	subrr => subrr,
	T => T,
	to_pc => to_pc,
	xorrr => xorrr,
	ZF => ZF
	);
	
process
begin
	source_operand <= source_operand + '1'; wait for 300 ns;
end process;

process
begin
	dest_operand <= '0'; wait for 200 ns;
	dest_operand <= '1'; wait for 200 ns;
end process;
	
process
begin
	SF <= '0'; wait for 200 ns;
	SF <= '1'; wait for 200 ns;
end process;

process
begin
	CF <= '0'; wait for 100 ns;
	CF <= '1'; wait for 100 ns;
end process;
	

process
begin
	ZF <= '0'; wait for 150 ns;
	ZF <= '1'; wait for 150 ns;
end process;	

process
begin
	OF1 <= '0'; wait for 250 ns;
	OF1 <= '1'; wait for 250 ns;
end process;	


PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
		  to_pc <= to_pc + '1'; wait for 40 ns;
		  
                                                      
END PROCESS ;   


process
begin
	 halt <= '1'; movri <= '0'; movrr <= '0';
	 addri <= '0'; addrr <= '0';
	 subri <= '0'; subrr <= '0';
	 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0'; 
	 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
	 cmprr <= '0'; jmprr <= '0'; jzrr <= '0';
	 wait for 500 ns;
	 halt <= '0'; movri <= '1'; movrr <= '0';
	 addri <= '0'; addrr <= '0';
	 subri <= '0'; subrr <= '0';
	 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0'; 
	 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
	 cmprr <= '0'; jmprr <= '0'; jzrr <= '0';
	 wait for 500 ns;
	 halt <= '0'; movri <= '0'; movrr <= '1';
	 addri <= '0'; addrr <= '0';
	 subri <= '0'; subrr <= '0';
	 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0'; 
	 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
	 cmprr <= '0'; jmprr <= '0'; jzrr <= '0';
	 wait for 500 ns;
	 halt <= '0'; movri <= '0'; movrr <= '0';
	 addri <= '1'; addrr <= '0';
	 subri <= '0'; subrr <= '0';
	 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0'; 
	 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
	 cmprr <= '0'; jmprr <= '0'; jzrr <= '0';
	 wait for 500 ns;
	 halt <= '0'; movri <= '0'; movrr <= '0';
	 addri <= '0'; addrr <= '1';
	 subri <= '0'; subrr <= '0';
	 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0'; 
	 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
	 cmprr <= '0'; jmprr <= '0'; jzrr <= '0';
	 wait for 500 ns;
	 halt <= '0'; movri <= '0'; movrr <= '0';
	 addri <= '0'; addrr <= '0';
	 subri <= '1'; subrr <= '0';
	 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0'; 
	 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
	 cmprr <= '0'; jmprr <= '0'; jzrr <= '0';
	 wait for 500 ns;
	 halt <= '0'; movri <= '0'; movrr <= '0';
	 addri <= '0'; addrr <= '0';
	 subri <= '0'; subrr <= '1';
	 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0'; 
	 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
	 cmprr <= '0'; jmprr <= '0'; jzrr <= '0';
	 wait for 500 ns;
	 halt <= '0'; movri <= '0'; movrr <= '0';
	 addri <= '0'; addrr <= '0';
	 subri <= '0'; subrr <= '0';
	 andrr <= '1'; orrr <= '0'; xorrr <= '0'; notrr <= '0'; 
	 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
	 cmprr <= '0'; jmprr <= '0'; jzrr <= '0';
	 wait for 500 ns;
	 halt <= '0'; movri <= '0'; movrr <= '0';
	 addri <= '0'; addrr <= '0';
	 subri <= '0'; subrr <= '0';
	 andrr <= '0'; orrr <= '1'; xorrr <= '0'; notrr <= '0'; 
	 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
	 cmprr <= '0'; jmprr <= '0'; jzrr <= '0';
	 wait for 500 ns;
	 halt <= '0'; movri <= '0'; movrr <= '0';
	 addri <= '0'; addrr <= '0';
	 subri <= '0'; subrr <= '0';
	 andrr <= '0'; orrr <= '0'; xorrr <= '1'; notrr <= '0'; 
	 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
	 cmprr <= '0'; jmprr <= '0'; jzrr <= '0';
	 wait for 500 ns;
	 halt <= '0'; movri <= '0'; movrr <= '0';
	 addri <= '0'; addrr <= '0';
	 subri <= '0'; subrr <= '0';
	 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '1'; 
	 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
	 cmprr <= '0'; jmprr <= '0'; jzrr <= '0';
	 wait for 500 ns;
	 halt <= '1'; movri <= '0'; movrr <= '0';
	 addri <= '0'; addrr <= '0';
	 subri <= '0'; subrr <= '0';
	 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0'; 
	 shlrr <= '1'; shrrr <= '0'; sarrr <= '0';
	 cmprr <= '0'; jmprr <= '0'; jzrr <= '0';
	 wait for 500 ns;
	 halt <= '0'; movri <= '0'; movrr <= '0';
	 addri <= '0'; addrr <= '0';
	 subri <= '0'; subrr <= '0';
	 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0'; 
	 shlrr <= '0'; shrrr <= '1'; sarrr <= '0';
	 cmprr <= '0'; jmprr <= '0'; jzrr <= '0';
	 wait for 500 ns;
	 halt <= '0'; movri <= '0'; movrr <= '0';
	 addri <= '0'; addrr <= '0';
	 subri <= '0'; subrr <= '0';
	 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0'; 
	 shlrr <= '0'; shrrr <= '0'; sarrr <= '1';
	 cmprr <= '0'; jmprr <= '0'; jzrr <= '0';
	 wait for 500 ns;
	 halt <= '0'; movri <= '0'; movrr <= '0';
	 addri <= '0'; addrr <= '0';
	 subri <= '0'; subrr <= '0';
	 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0'; 
	 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
	 cmprr <= '1'; jmprr <= '0'; jzrr <= '0';
	 wait for 500 ns;
	 halt <= '0'; movri <= '0'; movrr <= '0';
	 addri <= '0'; addrr <= '0';
	 subri <= '0'; subrr <= '0';
	 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0'; 
	 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
	 cmprr <= '0'; jmprr <= '1'; jzrr <= '0';
	 wait for 500 ns;
	 halt <= '0'; movri <= '0'; movrr <= '0';
	 addri <= '0'; addrr <= '0';
	 subri <= '0'; subrr <= '0';
	 andrr <= '0'; orrr <= '0'; xorrr <= '0'; notrr <= '0'; 
	 shlrr <= '0'; shrrr <= '0'; sarrr <= '0';
	 cmprr <= '0'; jmprr <= '0'; jzrr <= '1';
	 wait for 500 ns;
	 
	 
end process;
                                        
always : PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
		  T <= T(6 downto 0) & T(7); wait for 50 ns;
END PROCESS always;                                          
END controller_arch;
