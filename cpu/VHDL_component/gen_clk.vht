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
-- Generated on "06/29/2021 20:48:13"
                                                            
-- Vhdl Test Bench template for design  :  gen_clk
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY gen_clk_vhd_tst IS
END gen_clk_vhd_tst;
ARCHITECTURE gen_clk_arch OF gen_clk_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL clk_in : STD_LOGIC;
SIGNAL clk_out : STD_LOGIC;
SIGNAL en : STD_LOGIC;
COMPONENT gen_clk
	PORT (
	clk_in : IN STD_LOGIC;
	clk_out : BUFFER STD_LOGIC;
	en : IN STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : gen_clk
	PORT MAP (
-- list connections between master ports and signals
	clk_in => clk_in,
	clk_out => clk_out,
	en => en
	);
init : PROCESS                                               
-- variable declarations                                     
BEGIN                                                        
        -- code that executes only once    
				en <= '1'; wait for 30 ns;
				en <= '0';
				wait for 300 ns;
				en <= '1';
				wait for 30 ns;
				en <= '0';
WAIT;                                                       
END PROCESS init;                                           
always : PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
		  clk_in <= '0'; wait for 10 ns;
		  clk_in <= '1'; wait for 10 ns;                          
END PROCESS always;                                           
END gen_clk_arch;
