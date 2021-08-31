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
-- Generated on "06/29/2021 20:08:47"
                                                            
-- Vhdl Test Bench template for design  :  pc
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                
use ieee.std_LOGIC_unsigned.all;
ENTITY pc_vhd_tst IS
END pc_vhd_tst;
ARCHITECTURE pc_arch OF pc_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL clk : STD_LOGIC;
SIGNAL clr : STD_LOGIC;
SIGNAL ijmp : STD_LOGIC;
SIGNAL ipc : STD_LOGIC;
SIGNAL pc_out : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL target : STD_LOGIC_VECTOR(7 DOWNTO 0) := (others => '0');
COMPONENT pc
	PORT (
	clk : IN STD_LOGIC;
	clr : IN STD_LOGIC;
	ijmp : IN STD_LOGIC;
	ipc : IN STD_LOGIC;
	pc_out : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
	target : IN STD_LOGIC_VECTOR(7 DOWNTO 0)
	);
END COMPONENT;
BEGIN
	i1 : pc
	PORT MAP (
-- list connections between master ports and signals
	clk => clk,
	clr => clr,
	ijmp => ijmp,
	ipc => ipc,
	pc_out => pc_out,
	target => target
	);
-- clk
t_prcs_clk: PROCESS
BEGIN
LOOP
	clk <= '0';
	WAIT FOR 10000 ps;
	clk <= '1';
	WAIT FOR 10000 ps;
	IF (NOW >= 1000000 ps) THEN WAIT; END IF;
END LOOP;
END PROCESS t_prcs_clk;

-- clr
t_prcs_clr: PROCESS
BEGIN
	clr <= '0';
	WAIT FOR 30000 ps;
	clr <= '1';
WAIT;
END PROCESS t_prcs_clr;

-- ijmp
t_prcs_ijmp: PROCESS
BEGIN
	ijmp <= '1';
	WAIT FOR 140000 ps;
	ijmp <= '0';
	WAIT FOR 190000 ps;
	ijmp <= '1';
WAIT;
END PROCESS t_prcs_ijmp;

-- ipc
t_prcs_ipc: PROCESS
BEGIN
LOOP
	ipc <= '0';
	WAIT FOR 100000 ps;
	ipc <= '1';
	WAIT FOR 100000 ps;
	IF (NOW >= 1000000 ps) THEN WAIT; END IF;
END LOOP;
END PROCESS t_prcs_ipc;                                     
always : PROCESS                                              
-- optional sensitivity list                                  
-- (        )                                                 
-- variable declarations                                      
BEGIN                                                         
        -- code executes for every event on sensitivity list  
      wait for 30 ns;
	   target <= target + '1';
END PROCESS always;                                          
END pc_arch;
