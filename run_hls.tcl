# Create a project
open_component -reset component_fixed_point_log2 -flow_target vivado

# Add design files
add_files fxp_log2_top.cpp
# Add test bench & files
add_files -tb fxp_log2_test.cpp

# Set the top-level function
set_top fxp_log2_top

# ########################################################
# Create a solution
# Define technology and clock rate
# set_part  {xcvu9p-flga2104-2-i}
set_part xc7s100fgga676-1
create_clock -period 5

# Set variable to select which steps to execute
set hls_exec 2


csim_design
# Set any optimization directives
set_directive_interface -register fxp_log2_top in_val
set_directive_interface -mode ap_ctrl_hs -register fxp_log2_top return
set_directive_pipeline fxp_log2_top

# End of directives

if {$hls_exec == 1} {
	# Run Synthesis and Exit
	csynth_design
	
} elseif {$hls_exec == 2} {
	# Run Synthesis, RTL Simulation and Exit
	csynth_design
	
	cosim_design
} elseif {$hls_exec == 3} {
	# Run Synthesis, RTL Simulation, RTL implementation and Exit
	csynth_design
	
	cosim_design
   export_design
} else {
	# Default is to exit after setup
}

exit


