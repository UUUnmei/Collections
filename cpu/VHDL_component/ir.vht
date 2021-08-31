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
-- Generated on "06/29/2021 21:24:38"
                                                            
-- Vhdl Test Bench template for design  :  ir
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                
use ieee.std_LOGIC_unsigned.all;
ENTITY ir_vhd_tst IS
END ir_vhd_tst;
ARCHITECTURE ir_arch OF ir_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL addri : STD_LOGIC;
SIGNAL addrr : STD_LOGIC;
SIGNAL andrr : STD_LOGIC;
SIGNAL clk : STD_LOGIC;
SIGNAL cmprr : STD_LOGIC;
SIGNAL d : STD_LOGIC_VECTOR(15 DOWNTO 0) := (others => '0');
SIGNAL dest_operand : STD_LOGIC;
SIGNAL halt : STD_LOGIC;
SIGNAL iir : STD_LOGIC;
SIGNAL jmprr : STD_LOGIC;
SIGNAL jzrr : STD_LOGIC;
SIGNAL movri : STD_LOGIC;
SIGNAL movrr : STD_LOGIC;
SIGNAL notrr : STD_LOGIC;
SIGNAL orrr : STD_LOGIC;
SIGNAL out_to_pc : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL sarrr : STD_LOGIC;
SIGNAL shlrr : STD_LOGIC;
SIGNAL shrrr : STD_LOGIC;
SIGNAL source_operand : STD_LOGIC_VECTOR(1 DOWNTO 0);
SIGNAL subri : STD_LOGIC;
SIGNAL subrr : STD_LOGIC;
SIGNAL xorrr : STD_LOGIC;
COMPONENT ir
	PORT (
	addri : OUT STD_LOGIC;
	addrr : OUT STD_LOGIC;
	andrr : OUT STD_LOGIC;
	clk : IN STD_LOGIC;
	cmprr : OUT STD_LOGIC;
	d : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	dest_operand : OUT STD_LOGIC;
	halt : OUT STD_LOGIC;
	iir : IN STD_LOGIC;
	jmprr : OUT STD_LOGIC;
	jzrr : OUT STD_LOGIC;
	movri : OUT STD_LOGIC;
	movrr : OUT STD_LOGIC;
	notrr : OUT STD_LOGIC;
	orrr : OUT STD_LOGIC;
	out_to_pc : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
	sarrr : OUT STD_LOGIC;
	shlrr : OUT STD_LOGIC;
	shrrr : OUT STD_LOGIC;
	source_operand : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
	subri : OUT STD_LOGIC;
	subrr : OUT STD_LOGIC;
	xorrr : OUT STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : ir
	PORT MAP (
-- list connections between master ports and signals
	addri => addri,
	addrr => addrr,
	andrr => andrr,
	clk => clk,
	cmprr => cmprr,
	d => d,
	dest_operand => dest_operand,
	halt => halt,
	iir => iir,
	jmprr => jmprr,
	jzrr => jzrr,
	movri => movri,
	movrr => movrr,
	notrr => notrr,
	orrr => orrr,
	out_to_pc => out_to_pc,
	sarrr => sarrr,
	shlrr => shlrr,
	shrrr => shrrr,
	source_operand => source_operand,
	subri => subri,
	subrr => subrr,
	xorrr => xorrr
	);
init : PROCESS                                               
-- variable declarations                                     
BEGIN                                                        
        -- code that executes only once    
				iir <= '1'; wait for 30 ns;
				iir <= '0';
				wait for 300 ns;
				iir <= '1';
				wait for 30 ns;
				iir <= '0';
WAIT;                                                       
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
		  d <= d + '1'; wait for 40 ns;
		  
                                                      
END PROCESS ;                                            
END ir_arch;
