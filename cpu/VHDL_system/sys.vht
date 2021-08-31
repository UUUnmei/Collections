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
-- Generated on "07/05/2021 21:03:47"
                                                            
-- Vhdl Test Bench template for design  :  sys
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY sys_vhd_tst IS
END sys_vhd_tst;
ARCHITECTURE sys_arch OF sys_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL addri2 : STD_LOGIC;
SIGNAL addrr2 : STD_LOGIC;
SIGNAL bus_out : STD_LOGIC_VECTOR(15 DOWNTO 0) ;
SIGNAL CF : STD_LOGIC;
SIGNAL clk : STD_LOGIC;
SIGNAL clk2 : STD_LOGIC;
SIGNAL clr : STD_LOGIC;
SIGNAL ea2 : STD_LOGIC_VECTOR(1 DOWNTO 0);
SIGNAL ealu2 : STD_LOGIC;
SIGNAL edr2 : STD_LOGIC;
SIGNAL halt2 : STD_LOGIC;
SIGNAL ia2 : STD_LOGIC_VECTOR(1 DOWNTO 0);
SIGNAL idr2 : STD_LOGIC;
SIGNAL iir2 : STD_LOGIC;
SIGNAL imar2 : STD_LOGIC;
SIGNAL ipc2 : STD_LOGIC;
SIGNAL movri2 : STD_LOGIC;
SIGNAL movrr2 : STD_LOGIC;
SIGNAL OF1 : STD_LOGIC;
SIGNAL r1_to_alu2 : STD_LOGIC_VECTOR(15 DOWNTO 0) := (others => '0');
SIGNAL r2_to_alu2 : STD_LOGIC_VECTOR(15 DOWNTO 0) := (others => '0');
SIGNAL SF : STD_LOGIC;
SIGNAL subri2 : STD_LOGIC;
SIGNAL subrr2 : STD_LOGIC;
SIGNAL TT : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL ZF : STD_LOGIC;
COMPONENT sys
	PORT (
	addri2 : OUT STD_LOGIC;
	addrr2 : OUT STD_LOGIC;
	bus_out : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);
	CF : OUT STD_LOGIC;
	clk : IN STD_LOGIC;
	clk2 : OUT STD_LOGIC;
	clr : IN STD_LOGIC;
	ea2 : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
	ealu2 : OUT STD_LOGIC;
	edr2 : OUT STD_LOGIC;
	halt2 : OUT STD_LOGIC;
	ia2 : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
	idr2 : OUT STD_LOGIC;
	iir2 : OUT STD_LOGIC;
	imar2 : OUT STD_LOGIC;
	ipc2 : OUT STD_LOGIC;
	movri2 : OUT STD_LOGIC;
	movrr2 : OUT STD_LOGIC;
	OF1 : OUT STD_LOGIC;
	r1_to_alu2 : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);
	r2_to_alu2 : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);
	SF : OUT STD_LOGIC;
	subri2 : OUT STD_LOGIC;
	subrr2 : OUT STD_LOGIC;
	TT : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
	ZF : OUT STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : sys
	PORT MAP (
-- list connections between master ports and signals
	addri2 => addri2,
	addrr2 => addrr2,
	bus_out => bus_out,
	CF => CF,
	clk => clk,
	clk2 => clk2,
	clr => clr,
	ea2 => ea2,
	ealu2 => ealu2,
	edr2 => edr2,
	halt2 => halt2,
	ia2 => ia2,
	idr2 => idr2,
	iir2 => iir2,
	imar2 => imar2,
	ipc2 => ipc2,
	movri2 => movri2,
	movrr2 => movrr2,
	OF1 => OF1,
	r1_to_alu2 => r1_to_alu2,
	r2_to_alu2 => r2_to_alu2,
	SF => SF,
	subri2 => subri2,
	subrr2 => subrr2,
	TT => TT,
	ZF => ZF
	);
init : PROCESS                                               
-- variable declarations                                     
BEGIN                                                        
        -- code that executes only once  
			clr <= '0'; wait for 45 ns;
			clr <= '1';
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
END sys_arch;
