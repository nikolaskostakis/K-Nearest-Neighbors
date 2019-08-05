# cat benchmarks/drawbuffer/drawbuffer_1.txt
read_drawbuffer_output benchmarks/drawbuffer/drawbuffer_1.txt
# print_element_array
create_kdTree
# print_array
find_nearest_neighbours -k 1 -point 15 23
find_nearest_neighbours -k 5 -point 15 32
# find_neighbours_within_radius -r 0.4 -point 3 3
# find_neighbours_within_radius -r 0.1 -point 25 32
find_neighbours_within_radius -r 10 -point 25 32
# find_neighbours_within_radius -r 40 -point 25 32
start_gui
clear_elements
exit
# exit
# find_nearest_neighbours -k 1 -point 2.4 4.4
