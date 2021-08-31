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
-- Generated on "06/29/2021 20:01:46"
                                                            
-- Vhdl Test Bench template for design  :  mar
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                
use ieee.std_LOGIC_unsigned.all;
ENTITY mar_vhd_tst IS
END mar_vhd_tst;
ARCHITECTURE mar_arch OF mar_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL clk : STD_LOGIC;
SIGNAL imar : STD_LOGIC;
SIGNAL mar_in : STD_LOGIC_VECTOR(7 DOWNTO 0) := (others => '0');
SIGNAL mar_out : STD_LOGIC_VECTOR(7 DOWNTO 0);
COMPONENT mar
	PORT (
	clk : IN STD_LOGIC;
	imar : IN STD_LOGIC;
	mar_in : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
	mar_out : OUT STD_LOGIC_VECTOR(7 DOWNTO 0)
	);
END COMPONENT;
BEGIN
	i1 : mar
	PORT MAP (
-- list connections between master ports and signals
	clk => clk,
	imar => imar,
	mar_in => mar_in,
	mar_out => mar_out
	);
                                          
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
		  imar <= '1'; wait for 300 ns;
		  imar <= '0'; wait for 300 ns;
                                                       
END PROCESS ;
 PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
		  wait for  30 ns;	
        mar_in <= mar_in + '1';                                                     
END PROCESS ;                                        
END mar_arch;
