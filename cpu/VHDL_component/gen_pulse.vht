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
-- Generated on "06/29/2021 20:44:15"
                                                            
-- Vhdl Test Bench template for design  :  gen_pulse
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY gen_pulse_vhd_tst IS
END gen_pulse_vhd_tst;
ARCHITECTURE gen_pulse_arch OF gen_pulse_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL clk : STD_LOGIC;
SIGNAL clr : STD_LOGIC;
SIGNAL T : STD_LOGIC_VECTOR(7 DOWNTO 0);
COMPONENT gen_pulse
	PORT (
	clk : IN STD_LOGIC;
	clr : IN STD_LOGIC;
	T : BUFFER STD_LOGIC_VECTOR(7 DOWNTO 0)
	);
END COMPONENT;
BEGIN
	i1 : gen_pulse
	PORT MAP (
-- list connections between master ports and signals
	clk => clk,
	clr => clr,
	T => T
	);
init : PROCESS                                               
-- variable declarations                                     
BEGIN                                                        
        -- code that executes only once    
				clr <= '0'; wait for 30 ns;
				clr <= '1';
				wait for 300 ns;
				clr <= '0';
				wait for 30 ns;
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
END gen_pulse_arch;
