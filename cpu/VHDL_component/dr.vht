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
-- Generated on "06/29/2021 20:14:21"
                                                            
-- Vhdl Test Bench template for design  :  dr
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                
use ieee.std_logic_unsigned.all;
ENTITY dr_vhd_tst IS
END dr_vhd_tst;
ARCHITECTURE dr_arch OF dr_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL clk : STD_LOGIC;
SIGNAL dr_in : STD_LOGIC_VECTOR(15 DOWNTO 0)  := (others => '0');
SIGNAL dr_out : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL edr : STD_LOGIC;
SIGNAL idr : STD_LOGIC;
COMPONENT dr
	PORT (
	clk : IN STD_LOGIC;
	dr_in : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	dr_out : BUFFER STD_LOGIC_VECTOR(15 DOWNTO 0);
	edr : IN STD_LOGIC;
	idr : IN STD_LOGIC
	);
END COMPONENT;
BEGIN
	i1 : dr
	PORT MAP (
-- list connections between master ports and signals
	clk => clk,
	dr_in => dr_in,
	dr_out => dr_out,
	edr => edr,
	idr => idr
	);
	
PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
			clk <= '0'; wait for 10 ns;
			clk <= '1'; wait for 10 ns;
END PROCESS ; 
	
always : PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
			wait for 30 ns;
			dr_in <= dr_in + '1';
END PROCESS always;      
                 
-- idr
PROCESS
BEGIN
	idr <= '0';
	WAIT FOR 60000 ps;
	idr <= '1';
	WAIT FOR 80000 ps;
	idr <= '0';
	WAIT FOR 120000 ps;
	idr <= '1';
	WAIT FOR 80000 ps;
	idr <= '0';
WAIT;
END PROCESS ;

-- edr
 PROCESS
BEGIN
	edr <= '1';
	WAIT FOR 80000 ps;
	edr <= '0';
	WAIT FOR 320000 ps;
	edr <= '1';
	WAIT FOR 70000 ps;
	edr <= '0';
WAIT;
END PROCESS ;                      
                                          
END dr_arch;
