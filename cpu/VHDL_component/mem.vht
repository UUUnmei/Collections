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
-- Generated on "06/29/2021 19:33:39"
                                                            
-- Vhdl Test Bench template for design  :  mem
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                
use ieee.std_logic_unsigned.all;

ENTITY mem_vhd_tst IS
END mem_vhd_tst;
ARCHITECTURE mem_arch OF mem_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL addr : STD_LOGIC_VECTOR(7 DOWNTO 0) := (others => '0');
SIGNAL cs : STD_LOGIC;
SIGNAL mem_out : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL wr : STD_LOGIC;
COMPONENT mem
	PORT (
	addr : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
	cs : IN STD_LOGIC;
	mem_out : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);
	wr : IN STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : mem
	PORT MAP (
-- list connections between master ports and signals
	addr => addr,
	cs => cs,
	mem_out => mem_out,
	wr => wr
	);
	
-- cs
t_prcs_cs: PROCESS
BEGIN
	cs <= '0';
	WAIT FOR 100000 ps;
	cs <= '1';
	WAIT FOR 120000 ps;
	cs <= '0';
WAIT;
END PROCESS t_prcs_cs;

-- wr
t_prcs_wr: PROCESS
BEGIN
	wr <= '1';
	WAIT FOR 180000 ps;
	wr <= '0';
	WAIT FOR 100000 ps;
	wr <= '1';
WAIT;
END PROCESS t_prcs_wr;
	
                                        
always : PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
		wait for  30 ns;	
		addr <= addr + '1'; 													
END PROCESS always;                                          
END mem_arch;
