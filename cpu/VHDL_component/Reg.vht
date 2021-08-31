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
-- Generated on "06/29/2021 20:23:24"
                                                            
-- Vhdl Test Bench template for design  :  Reg
-- 
-- Simulation tool : ModelSim-Altera (VHDL)
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY Reg_vhd_tst IS
END Reg_vhd_tst;
ARCHITECTURE Reg_arch OF Reg_vhd_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL clk : STD_LOGIC;
SIGNAL d : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL in_from_dbus : STD_LOGIC;
SIGNAL out_to_dbus : STD_LOGIC;
SIGNAL qalu : STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL qbus : STD_LOGIC_VECTOR(15 DOWNTO 0);
COMPONENT Reg
	PORT (
	clk : IN STD_LOGIC;
	d : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
	in_from_dbus : IN STD_LOGIC;
	out_to_dbus : IN STD_LOGIC;
	qalu : OUT STD_LOGIC_VECTOR(15 DOWNTO 0);
	qbus : OUT STD_LOGIC_VECTOR(15 DOWNTO 0)
	);
END COMPONENT;
BEGIN
	i1 : Reg
	PORT MAP (
-- list connections between master ports and signals
	clk => clk,
	d => d,
	in_from_dbus => in_from_dbus,
	out_to_dbus => out_to_dbus,
	qalu => qalu,
	qbus => qbus
	);
-- clk
t_prcs_clk: PROCESS
BEGIN
LOOP
	clk <= '0';
	WAIT FOR 25000 ps;
	clk <= '1';
	WAIT FOR 25000 ps;
	IF (NOW >= 1000000 ps) THEN WAIT; END IF;
END LOOP;
END PROCESS t_prcs_clk;
-- d[15]
t_prcs_d_15: PROCESS
BEGIN
	d(15) <= '0';
WAIT;
END PROCESS t_prcs_d_15;
-- d[14]
t_prcs_d_14: PROCESS
BEGIN
	d(14) <= '0';
WAIT;
END PROCESS t_prcs_d_14;
-- d[13]
t_prcs_d_13: PROCESS
BEGIN
	d(13) <= '0';
WAIT;
END PROCESS t_prcs_d_13;
-- d[12]
t_prcs_d_12: PROCESS
BEGIN
	d(12) <= '0';
WAIT;
END PROCESS t_prcs_d_12;
-- d[11]
t_prcs_d_11: PROCESS
BEGIN
	d(11) <= '0';
	WAIT FOR 20000 ps;
	d(11) <= '1';
	WAIT FOR 150000 ps;
	d(11) <= '0';
WAIT;
END PROCESS t_prcs_d_11;
-- d[10]
t_prcs_d_10: PROCESS
BEGIN
	d(10) <= '0';
	WAIT FOR 20000 ps;
	d(10) <= '1';
	WAIT FOR 150000 ps;
	d(10) <= '0';
WAIT;
END PROCESS t_prcs_d_10;
-- d[9]
t_prcs_d_9: PROCESS
BEGIN
	d(9) <= '0';
	WAIT FOR 20000 ps;
	d(9) <= '1';
	WAIT FOR 150000 ps;
	d(9) <= '0';
WAIT;
END PROCESS t_prcs_d_9;
-- d[8]
t_prcs_d_8: PROCESS
BEGIN
	d(8) <= '0';
	WAIT FOR 20000 ps;
	d(8) <= '1';
	WAIT FOR 150000 ps;
	d(8) <= '0';
WAIT;
END PROCESS t_prcs_d_8;
-- d[7]
t_prcs_d_7: PROCESS
BEGIN
	d(7) <= '0';
	WAIT FOR 20000 ps;
	d(7) <= '1';
	WAIT FOR 150000 ps;
	d(7) <= '0';
WAIT;
END PROCESS t_prcs_d_7;
-- d[6]
t_prcs_d_6: PROCESS
BEGIN
	d(6) <= '0';
	WAIT FOR 20000 ps;
	d(6) <= '1';
	WAIT FOR 150000 ps;
	d(6) <= '0';
WAIT;
END PROCESS t_prcs_d_6;
-- d[5]
t_prcs_d_5: PROCESS
BEGIN
	d(5) <= '0';
	WAIT FOR 20000 ps;
	d(5) <= '1';
	WAIT FOR 150000 ps;
	d(5) <= '0';
WAIT;
END PROCESS t_prcs_d_5;
-- d[4]
t_prcs_d_4: PROCESS
BEGIN
	d(4) <= '0';
	WAIT FOR 20000 ps;
	d(4) <= '1';
	WAIT FOR 150000 ps;
	d(4) <= '0';
WAIT;
END PROCESS t_prcs_d_4;
-- d[3]
t_prcs_d_3: PROCESS
BEGIN
	d(3) <= '0';
	WAIT FOR 390000 ps;
	d(3) <= '1';
	WAIT FOR 90000 ps;
	d(3) <= '0';
WAIT;
END PROCESS t_prcs_d_3;
-- d[2]
t_prcs_d_2: PROCESS
BEGIN
	d(2) <= '0';
	WAIT FOR 390000 ps;
	d(2) <= '1';
	WAIT FOR 90000 ps;
	d(2) <= '0';
WAIT;
END PROCESS t_prcs_d_2;
-- d[1]
t_prcs_d_1: PROCESS
BEGIN
	d(1) <= '0';
	WAIT FOR 390000 ps;
	d(1) <= '1';
	WAIT FOR 90000 ps;
	d(1) <= '0';
WAIT;
END PROCESS t_prcs_d_1;
-- d[0]
t_prcs_d_0: PROCESS
BEGIN
	d(0) <= '0';
	WAIT FOR 390000 ps;
	d(0) <= '1';
	WAIT FOR 90000 ps;
	d(0) <= '0';
WAIT;
END PROCESS t_prcs_d_0;

-- in_from_dbus
t_prcs_in_from_dbus: PROCESS
BEGIN
	in_from_dbus <= '1';
	WAIT FOR 70000 ps;
	in_from_dbus <= '0';
	WAIT FOR 160000 ps;
	in_from_dbus <= '1';
	WAIT FOR 240000 ps;
	in_from_dbus <= '0';
	WAIT FOR 60000 ps;
	in_from_dbus <= '1';
WAIT;
END PROCESS t_prcs_in_from_dbus;

-- out_to_dbus
t_prcs_out_to_dbus: PROCESS
BEGIN
	out_to_dbus <= '0';
	WAIT FOR 210000 ps;
	out_to_dbus <= '1';
	WAIT FOR 310000 ps;
	out_to_dbus <= '0';
WAIT;
END PROCESS t_prcs_out_to_dbus;                                  
END Reg_arch;
