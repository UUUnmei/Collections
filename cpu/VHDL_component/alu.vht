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
-- Generated on "06/29/2021 21:06:14"
                                                            
-- Vhdl Test Bench template for design  :  alu
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                
use ieee.std_LOGIC_unsigned.all;
ENTITY alu_vhd_tst IS
END alu_vhd_tst;
ARCHITECTURE alu_arch OF alu_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL A : STD_LOGIC_VECTOR(15 DOWNTO 0):= (others => '0');
SIGNAL B : STD_LOGIC_VECTOR(15 DOWNTO 0) := (others => '0');
SIGNAL CF : STD_LOGIC;
SIGNAL clk : STD_LOGIC;
SIGNAL ealu : STD_LOGIC;
SIGNAL iadd : STD_LOGIC;
SIGNAL iand : STD_LOGIC;
SIGNAL icmp : STD_LOGIC;
SIGNAL inot : STD_LOGIC;
SIGNAL ior : STD_LOGIC;
SIGNAL isar : STD_LOGIC;
SIGNAL ishl : STD_LOGIC;
SIGNAL ishr : STD_LOGIC;
SIGNAL isub : STD_LOGIC;
SIGNAL ixor : STD_LOGIC;
SIGNAL OF1 : STD_LOGIC;
SIGNAL res : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL SF : STD_LOGIC;
SIGNAL ZF : STD_LOGIC;
COMPONENT alu
	PORT (
	A : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	B : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	CF : OUT STD_LOGIC;
	clk : IN STD_LOGIC;
	ealu : IN STD_LOGIC;
	iadd : IN STD_LOGIC;
	iand : IN STD_LOGIC;
	icmp : IN STD_LOGIC;
	inot : IN STD_LOGIC;
	ior : IN STD_LOGIC;
	isar : IN STD_LOGIC;
	ishl : IN STD_LOGIC;
	ishr : IN STD_LOGIC;
	isub : IN STD_LOGIC;
	ixor : IN STD_LOGIC;
	OF1 : OUT STD_LOGIC;
	res : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);
	SF : OUT STD_LOGIC;
	ZF : OUT STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : alu
	PORT MAP (
-- list connections between master ports and signals
	A => A,
	B => B,
	CF => CF,
	clk => clk,
	ealu => ealu,
	iadd => iadd,
	iand => iand,
	icmp => icmp,
	inot => inot,
	ior => ior,
	isar => isar,
	ishl => ishl,
	ishr => ishr,
	isub => isub,
	ixor => ixor,
	OF1 => OF1,
	res => res,
	SF => SF,
	ZF => ZF
	);
init : PROCESS                                               
-- variable declarations                                     
BEGIN                                                        
        -- code that executes only once        
			ealu <= '1'; wait for 100 ns;
			ealu <= '0'; wait for 100 ns;
                                                       
END PROCESS init;                                           
always : PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
		  clk <= '0'; wait for 10 ns;
		  clk <= '1'; wait for 10 ns;                          
END PROCESS always;   

PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
		  A <= A + '1'; wait for 100 ns;
                                                       
END PROCESS;   

PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
		  B <= B + '1'; wait for 40 ns;
		  
                                                      
END PROCESS ; 

process
begin
	iadd <= '1';	
	isub <= '1';
	iand <= '1';
	ior <= '1'; 
	ixor <= '1'; 
	inot <= '1'; 
	ishl <= '0'; 
	ishr <= '1';
	isar <= '1'; 
	icmp <= '1';
	wait for 500 ns;
	ishl <= '1';
	ishr <= '0';
	wait for 500 ns;
	ishr <= '1';
	isar <= '0';
	wait for 500 ns;
	isar <= '1';
	iand <= '0';
	wait for 500 ns;
	iand <= '1';
	ior <= '0';
	wait for 500 ns;
	ior <= '1';
	ixor <= '0';
	wait for 500 ns;
	ixor <= '1';
	inot <= '0';
	wait for 500 ns;
	inot <= '1';
	isub <= '0';
	wait for 500 ns;
	isub <= '1';
	iadd <= '0';
	wait for 500 ns;
	iadd <= '1';
	icmp <= '0';
	wait for 500 ns;
	icmp <= '1';
	
	
end process;                                         
END alu_arch;
